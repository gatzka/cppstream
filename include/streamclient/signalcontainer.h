#ifndef _HBM__STREAMING__SIGNALS
#define _HBM__STREAMING__SIGNALS

#include <unordered_map>
#include <vector>

#include <boost/function.hpp>

#ifdef _WIN32
#include "json/value.h"
#define ssize_t int
#else
#include <jsoncpp/json/value.h>
#endif

#include "subscribedsignal.h"

namespace hbm {
	namespace streaming {

		typedef std::function<void(SubscribedSignal& subscribedSignal, const std::string& method, const Json::Value& params)> SignalMetaCb_t;

		/// Contains all subscribed signals.
		/// Callback functions may be registered in order to get informed about signal related meta information and measured data.
		class SignalContainer {
		public:
			SignalContainer();

			/// \warning set callback function before calling start(), otherwise you will miss meta information received.
			void setSignalMetaCb(SignalMetaCb_t cb);

			/// \warning set callback function before subscribing signals, otherwise you will miss measured values received.
			void setDataCb(DataCb_t cb);

			/// new subscribed signals are added with arrival of initial meta information
			void processMetaInformation(unsigned int signalNumber, const std::string &method, const Json::Value& params);

			/// \return number of bytes processed or -1 if signal is unknown.
			ssize_t processMeasuredData(unsigned int signalNumber, unsigned char* data, size_t len);

			void clear();

		private:

			/// signal number is the key
			typedef std::unordered_map < unsigned int, SubscribedSignal > signals_t;

			/// processes measured data and keeps meta information about all subscribed signals
			signals_t m_subscribedsignals;

			SignalMetaCb_t m_signalMetaCb;
			DataCb_t m_dataCb;
		};
	}
}
#endif