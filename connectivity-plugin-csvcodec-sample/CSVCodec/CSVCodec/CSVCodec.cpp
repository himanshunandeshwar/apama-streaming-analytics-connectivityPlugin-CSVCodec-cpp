/**
* Title:        CSVCodec.cpp
* Description:  CSV Codec
* Author: Himanshu
*/
#include <sag_connectivity_plugins.hpp>
#include <map>
#include <string>
#include <sstream>
#include <iterator>
#include <iostream>

using namespace std;
using namespace com::softwareag::connectivity;

/**
* When Message is moving Towards Transport this codec converts the payload of Message in eventMap format
* to a Comma Seperated string payload of Message.
* When Message is moving Towards Host this codec converts the payload of Message in Comma Seperated String format
* to eventMap payload of Message.
* This Codec requires that the Event Fields are defined in the YAML configuration as show below.
* Also the it is required that a defaulTEventType should be configured in YAML as shown below.
* eg.
------------------------
Line.mon
------------------------
event Line {
string name;
integer number;
string email;
}

------------------------
connectivityplugin.yaml
------------------------
- apama.eventMap:
	defaultEventType: Line
	suppressLoopback: true
	allowMissing: true

- CSVCodec:
	csvHeader:
	 - name
	 - number
	 - email


Note - 
1) If the message has less number of values than the number of keys then empty values given be passed for remailing keys. (allowMissing is True).
2) If the mesage payload has more number of values than the number of keys configured in YAML then extra values will be discarded
*/

class CSVCodec :public AbstractCodec {
public:

	/*List of Keys*/
	list_t keys;

	/**
	* Constructor, passing parameters to AbstractCodec.
	*
	* The config and chainId are available as members on this
	* class for the sub-class to refer to if needed.
	*
	* @param params see product documentation
	*/

	CSVCodec(const CodecConstructorParameters &params) : AbstractCodec(params) {

		/*Find the csvHeader key in config file and populate keys list*/
		try {
			map_t::iterator headitr = config.find("csvHeader");

			/* If the csvHeader is not configured in YAML throw exception*/
			if (headitr == config.end()) {
				throw "csvHeader not found in YAML configuration";
			}

			/* If the header is passed as a string of comma seperated
			* eg. name,number,email
			*/
			if (headitr->second.type_tag() == SAG_DATA_STRING) {

				const char* keystr = get<const char*>(headitr->second);
				istringstream header(keystr);
				string token;
				while (getline(header, token, ',')) {
					keys.push_back(token);
				}
			}
			/* Else Populate keys list ased on the csvHeader configured in YAML */
			else {
				keys = MapHelper::getList(config, "csvHeader").copy();
			}

		}
		catch (string str) {
			cout << str;
			return;
		}
	}

	/**
	* Converts the message in CSV format to event map.
	*
	*
	* @param start see product documentation
	* @param end see product documentation
	*/

	virtual void sendBatchTowardsHost(Message *start, Message *end) {

		/*The Delimeter for the message*/
		char delimeter = ',';

		for (Message *it = start; it != end; ++it) {

			/*If the Input data is not is string format then skip this message*/
			if (it->getPayload().type_tag() != SAG_DATA_STRING)
				continue;

			/*Extract the String payload from the message*/
			const char * payload = get<const char *>(it->getPayload());

			/*Create a list of values from string delimited payload*/
			list_t vals;
			istringstream line(payload);
			string token;
			while (getline(line, token, delimeter)) {
				vals.push_back(token);
			}

			/*Create an event map with <key,value> pair*/
			/// size of both list must be same
			map_t eventMap;
			for (auto keyitr = keys.begin(), valitr = vals.begin(); keyitr != keys.end(); ++keyitr, ++valitr) {
				eventMap.insert(make_pair(keyitr->copy(), valitr->copy()));
			}
			it->setPayload(move(eventMap));
		}

		hostSide->sendBatchTowardsHost(start, end);
	}

	/**
	* Converts the message to CSV format from event map.
	*
	*
	* @param start see product documentation
	* @param end see product documentation
	*/

	virtual void sendBatchTowardsTransport(Message *start, Message *end) {

		for (Message *it = start; it != end; ++it) {

			/*If the Input data is not is map_t format then skip this message*/
			if (it->getPayload().type_tag() != SAG_DATA_MAP)
				continue;

			/*Extract the payload from the message*/
			map_t &payload = get<map_t>(it->getPayload());

			/*Convert the paload to a csv string*/
			std::ostringstream ss;
			for (auto litr = keys.begin(); litr != keys.end(); ++litr) {
				auto mitr = payload.find(*litr);
				if (mitr != payload.end() && mitr->second.type_tag() != SAG_DATA_LIST && mitr->second.type_tag() != SAG_DATA_MAP) {
					if (litr == keys.begin()) {
						ss << to_string(mitr->second);
					}
					else {
						ss << "," << to_string(mitr->second);
					}
				}
			}
			it->setPayload(move(ss.str()));
		}
		transportSide->sendBatchTowardsTransport(start, end);
	}
};

SAG_DECLARE_CONNECTIVITY_CODEC_CLASS(CSVCodec)
