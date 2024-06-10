#include <iostream>

#include "crow_all.h"
#include "sendMail.h"

void sendTestEmail() {
	MailServer ms = {
		"test@01e.cc",
		"39,:,WooDeR",
		"smtps://mail.barpos.net"
	};

	Mail mail = {
		"test@01e.cc",
		"Administrator",
		"Test Email",
		"<html>Test html message</html>",
		"Test text message"
	};

	mail.recipients.push_back({ "admin@barpos.net", "barpos.net" });
	mail.recipients.push_back({ "admin@barpos.dev", "barpos.dev" });

	bool res = sendMail(mail, ms);
	if (!res) {
		fmt::println("ERROR EMAIL NOT SENT!\n");
		return;
	}
	fmt::println("Email sent!\n");
}


int main(void)
{
	crow::SimpleApp app;

	CROW_ROUTE(app, "/")([]() {
		return "BarPos' mailing API";
		});

	CROW_ROUTE(app, "/sendMail").methods("POST"_method)([](const crow::request& req) {
		crow::json::rvalue bdy = crow::json::load(req.body);
		if (!bdy) return crow::response(400);
		
		if (!bdy["server"]["login"]) crow::response(400);
		if (!bdy["server"]["password"]) crow::response(400);
		if (!bdy["server"]["url"]) crow::response(400);

		if (!bdy["sender"]["name"]) crow::response(400);
		if (!bdy["sender"]["email"]) crow::response(400);
		
		if (!bdy["recipients"]) crow::response(400);
		if (!bdy["recipients"][0]) crow::response(400);
		for (auto rcv : bdy["recipients"]) {
			if (!rcv["name"]) crow::response(400);
			if (!rcv["email"]) crow::response(400);
		}

		if (!bdy["mail"]["subject"]) crow::response(400);
		if (!bdy["mail"]["html"]) crow::response(400);
		if (!bdy["mail"]["text"]) crow::response(400);

		MailServer ms = { 
			(std::string)bdy["server"]["login"],
			(std::string)bdy["server"]["password"],
			(std::string)bdy["server"]["url"]
		};
		Mail mail = {
			(std::string)bdy["sender"]["email"],
			(std::string)bdy["sender"]["name"],
			(std::string)bdy["mail"]["subject"],
			(std::string)bdy["mail"]["html"],
			(std::string)bdy["mail"]["text"]
		};
		for (auto rcv : bdy["recipients"]) {
			mail.recipients.push_back({ (std::string)rcv["email"], (std::string)rcv["name"] });
		}

		bool sent = sendMail(mail, ms);

		if (!sent) {
			return crow::response(500);
		}

		return crow::response(200, "Email sent!");
		});

	app.port(18081).multithreaded().run();

	return 0;
}
