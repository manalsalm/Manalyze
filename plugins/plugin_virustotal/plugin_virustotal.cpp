/*
    This file is part of Spike Guard.

    Spike Guard is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Spike Guard is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Spike Guard.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <boost/system/api_config.hpp>

#if defined BOOST_WINDOWS_API && !defined _WIN32_WINNT
	#define _WIN32_WINNT 0x0501
#endif

#include <stdio.h>
#include <boost/asio.hpp>

#include "plugin_framework/plugin_interface.h"
#include "plugin_framework/auto_register.h"

#include "color.h"
#include "hash-library/hashes.h"
#include "plugin_virustotal/json_spirit/json_spirit.h"

namespace bai = boost::asio::ip;
namespace js = json_spirit;

namespace plugin {

/**
 *	@brief	Queries VirusTotal for a given hash.
 *
 *	@param	const std::string& hash The hash of the program whose AV results we want.
 *	@param	const std::string& api_key The VirusTotal API key used to submit queries.
 *	@param	std::string& destination The string which will recieve the REST JSON response.
 *
 *	@return	Whether the query was completed successfully.
 */
bool query_virus_total(const std::string& hash, const std::string& api_key, std::string& destination);


class VirusTotalPlugin : public IPlugin
{
private:
	std::string _api_key;

public:
	VirusTotalPlugin()
	{
		FILE* f = fopen(".virustotal", "r");
		if (f == NULL) {
			PRINT_WARNING << "File .virustotal not found. No API key provided." << std::endl;
		}
		else
		{
			char read_key[65];
			read_key[64] = '\0';
			if (1 != fread(read_key, 64, 1, f)) {
				PRINT_WARNING << "Could not read VirusTotal API key!" << std::endl;
			}
			else if (!strncmp(read_key, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", 64)) {
				PRINT_WARNING << "Please edit .virustotal with your VirusTotal API key!" << std::endl;
			}
			else {
				_api_key = std::string(read_key);
			}
		}
		if (f != NULL) {
			fclose(f);
		}
	}

	int get_api_version() { return 1; }
	
	pString get_id() const { 
		return pString(new std::string("virustotal"));
	}

	pString get_description() const { 
		return pString(new std::string("Checks existing AV results on VirusTotal."));
	}

	pResult analyze(const sg::PE& pe) 
	{
		pResult res(new Result());

		if (_api_key == "") { // No API key provided.
			return res;
		}
		
		std::string sha256_hash = *hash::hash_file(*hash::ALL_DIGESTS[ALL_DIGESTS_SHA256], *pe.get_path());
		std::string json;

		if (!query_virus_total(sha256_hash, _api_key, json)) {
			return res;
		}

		// Parse the JSON.
		js::Value val;
		if (!js::read(json, val) || val.type() != js::obj_type) 
		{
			PRINT_ERROR << "Could not parse JSON retrieved from VirusTotal!" << std::endl;
			return res;
		}

		js::Object root = val.get_obj();
		unsigned int total = 0, positives = 0;
		std::string scan_date;
		for (js::Object::const_iterator it = root.begin() ; it != root.end() ; ++it)
		{
			if (it->name_ == "response_code") 
			{
				if (!it->value_.get_int()) // Response Code = 0: VT does not know the file.
				{
					res->set_level(Result::SUSPICIOUS); // Because VT knows all the files.
					res->set_summary("This file has never been scanned on VirusTotal.");
					return res;
				}
				else if (it->value_.get_int() == -2) // Response Code = -2: scan queued.
				{
					res->set_summary("A scan if the file is currently queued on VirusTotal.");
					return res;
				}
			}
			else if (it->name_ == "total") {
				total = it->value_.get_int();
			}
			else if (it->name_ == "positives") {
				positives = it->value_.get_int();
			}
			else if (it->name_ == "scan_date") {
				scan_date = it->value_.get_str();
			}
			else if (it->name_ == "scans")
			{
				// Iterate on each AV's report
				js::Object scans = it->value_.get_obj();
				for (js::Object::const_iterator it2 = scans.begin() ; it2 != scans.end() ; ++it2)
				{
					// Iterate on each report's elements
					js::Object scan = it2->value_.get_obj();
					for (js::Object::const_iterator it3 = scan.begin() ; it3 != scan.end() ; ++it3)
					{
						if (it3->name_ == "detected" && !it3->value_.get_bool()) { // Not detected by this AV
							break; // No result to read
						}
						else if (it3->name_ == "result")
						{
							std::stringstream av_result;
							av_result << it2->name_ << ": " << it3->value_.get_str();
							res->add_information(av_result.str());
							break; // The information has been found, ignore whatever is left
						}
					}
				}
			}
		}

		std::stringstream ss;
		ss << "VirusTotal score: " << positives << "/" << total << " (Scanned on " << scan_date << ")";
		res->set_summary(ss.str());
		if (positives == 0) {
			res->set_level(Result::SAFE);
		}
		else if (positives < 3) { // Allow reasonable doubt for paranoid AVs
			res->set_level(Result::SUSPICIOUS);
		}
		else {
			res->set_level(Result::MALICIOUS);
		}

		return res;
	}
};

// ----------------------------------------------------------------------------

bool query_virus_total(const std::string& hash, const std::string& api_key, std::string& destination)
{
	boost::asio::io_service io_service;

	// Get a list of endpoints corresponding to the server name.
	bai::tcp::resolver resolver(io_service);
	bai::tcp::resolver::query query("www.virustotal.com", "http");
	bai::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	bai::tcp::resolver::iterator end;

	// Try each endpoint until we successfully establish a connection.
	bai::tcp::socket socket(io_service);
	boost::system::error_code error = boost::asio::error::host_not_found;
	while (error && endpoint_iterator != end)
	{
		socket.close();
		socket.connect(*endpoint_iterator++, error);
	}
	if (error) 
	{
		PRINT_ERROR << "Could not resolve www.virustotal.com (plugin_virustotal)!" << std::endl;
		return false;
	}

	// Build the request
	boost::asio::streambuf request;
	std::stringstream ss;
	ss << "apikey=" << api_key << "&resource=" << hash << "\r\n";
	std::string post_data = ss.str();
	std::ostream request_stream(&request);
	request_stream << "POST /vtapi/v2/file/report HTTP/1.1\r\n";
	request_stream << "Host: www.virustotal.com \r\n";
	request_stream << "Content-Length: " << post_data.size() << "\r\n";
	request_stream << "Connection: close\r\n\r\n";
	request_stream << post_data;

	// Send the request.
	boost::asio::write(socket, request);

	// Read the response status line.
	boost::asio::streambuf response;
	boost::asio::read_until(socket, response, "\r\n");

	// Check that response is OK.
	std::istream response_stream(&response);
	std::string http_version;
	response_stream >> http_version;
	unsigned int status_code;
	response_stream >> status_code;
	std::string status_message;
	std::getline(response_stream, status_message);
	if (!response_stream || http_version.substr(0, 5) != "HTTP/")
	{
		PRINT_ERROR << "Received a malformed response from the server. (plugin_virustotal)" << std::endl;
		return false;
	}
	if (status_code != 200)
	{
		if (status_code == 204)	{
			PRINT_ERROR << "VirusTotal API request rate limit reached!" << std::endl;
		}
		else if (status_code == 403) {
			PRINT_ERROR << "VirusTotal API access denied. Please verify that your API key is valid." << std::endl;
		}
		else {
			PRINT_ERROR << "VirusTotal query returned with status code " << status_code << std::endl;
		}
		return false;
	}

	// Skip the response headers.
	std::string header;
	while (std::getline(response_stream, header) && header != "\r");

	// Write whatever content we already have to output.
	ss.str("");
	ss.clear();
	if (response.size() > 0) {
		ss << &response;
	}

	// Read until EOF, writing data to output as we go.
	while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error)) {
		ss << &response;
	}
	if (error != boost::asio::error::eof) {
		PRINT_ERROR << "Could not read the response (" << error << ")." << std::endl;
		return false;
	}

	destination = ss.str();
	return true;
}

// ----------------------------------------------------------------------------

extern "C"
{
	PLUGIN_API IPlugin* create() { return new VirusTotalPlugin(); }
	PLUGIN_API void destroy(IPlugin* p) { if (p) delete p; }
};

} // !namespace plugin