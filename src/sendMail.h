#pragma once

#include <stdio.h>
#include <string>
#include <vector>
#include <string.h>
#include <iostream>
#include <fstream>
#include <curl/curl.h>
#include <fmt/core.h>

struct MailServer {
	std::string login;
	std::string password;
	std::string server;
};

struct Recipient {
	std::string email;
	std::string name;
};

struct Mail {
	std::string fromEmail;
	std::string fromName;
	std::string subject;
	std::string htmlMessage;
	std::string textMessage;
	std::vector<Recipient> recipients;
};

bool sendMail(const Mail& mail, const MailServer&);