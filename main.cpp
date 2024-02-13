#include<iostream>
#include<pqxx/pqxx>
#include<Windows.h>
#pragma execution_character_set( "utf-8" )
class ClientDB
{
public:
	void CreateTable(pqxx::connection& conn)
	{

		pqxx::transaction tx(conn);
		tx.exec("CREATE TABLE IF NOT EXISTS ClientDB "
			"(id SERIAL PRIMARY KEY, "
			"firstName TEXT not null, "
			"lastName TEXT not null, "
			"email TEXT not null, ");
		tx.commit();
		pqxx::transaction tx01(conn);
		tx01.exec("CREATE TABLE IF NOT EXISTS ClientPhone "
			"(phone_id SERIAL PRIMRAY KEY, "
			"client_id INT not null REFERENCES CleintDB(id), "
			"phoneNumber VARCHAR(60))");
		tx01.commit();
	}
	void AddNew(pqxx::connection& conn)
	{
		std::string firstName, lastName, email, phoneNum;
		std::cout << " Enter firstname and lastname : ";
		std::cin >> firstName >> lastName;
		std::cout << " Enter email : ";
		std::cin >> email;
		std::cout << " Enter phone number: ";
		std::cin >> phoneNum;

		pqxx::transaction tx(conn);
		std::string query1 = "INSERT INTO ClientDB(id,firstName,lastName,enail) VALUES (default, '" + tx.esc(firstName) + "', '" + tx.esc(lastName) + "','" + tx.esc(email) + "') returning id;";
		auto result = tx.exec(query1);
		std::tuple<std::string> x = result[0].as<std::string>();
		tx.commit();
		pqxx::transaction tx01(conn);
		tx01.exec("INSERT INTO ClientPhone(client_id, phoneNubmer) VALUES('"+ tx01.esc(std::get<0>(x)) +"','" + tx.esc(phoneNum) + "')");
		tx01.commit();
	}
};
int main()
{
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);

	try
	{
		pqxx::connection conn("host=localhost "
			"port=5432 "
			"dbname=LessonCPP1 "
			"user=postgres "
			"password=jltnnffnjcjdyf");
		std::cout << "Connection exists " << std::endl;
		ClientDB clientdb;
		clientdb.CreateTable(conn);
		std::cout << " Table created " << std::endl;
		std::cout << " Add new client: " << std::endl;
		clientdb.AddNew(conn);
	}
	catch (std::exception& ex)

	{
		std::cout << " Something goes wrong!  " << ex.what() << std::endl;
	}
	return 0;
}












