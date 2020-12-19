#include <iostream>
#include "core/httpmanager.h"

using namespace std;

int main()
{
	//Url url("http://127.0.0.1:8888/statistic");
	Url url("http://127.0.0.1:8888/html");
//	Url url("http://127.0.0.1:8888/htm");
	Request req(url);
	HttpManager manager;
	Responce res = manager.httpGet(req);
	std::cout << "Status code : " << res.getStatus() << endl;
	cout << "Body : " << endl;
	std::cout << res.getBody() << std::endl;
	return 0;
}
