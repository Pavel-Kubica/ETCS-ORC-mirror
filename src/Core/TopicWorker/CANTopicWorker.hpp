/** @file CANTopicWorker.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Implementation of topic worker for handling CAN hexstring messages
 *
 *  ### Contributors
 *  cajantom
 */

#pragma once

#include "TopicWorker.hpp"
#include "MessageHandler.hpp"

class CANTopicWorker : public TopicWorker {
public:
	/**
	 * @param handler Message handler for the message of the topic
	 * @param jru For logging purposes
	 * @param topic Topic handled by the worker, to indicate what topics are done working
	 */
	CANTopicWorker(std::shared_ptr<MessageHandler> handler,
		           JRULoggerService* jru,
		           Topic topic);

private:
	void ProcessMessage(const std::string& message) override;

	std::shared_ptr<MessageHandler> messageHandler;
};
