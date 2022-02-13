// Class diagramming example...
#include <iostream>
#include <fstream>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

////
const std::string DATA_FILE_PATH = "TestData\\";
//////////////////
namespace Demo
{
	enum class CLASS_TYPE
	{
		UNKNOWN,
		A,
		B,
		C
	};
	//////////////////
	class A
	{
		enum class STATE
		{
			NA,
			STOP,
			GO
		};

		STATE state {};

	protected:

		void SomeProtectedFunc() {}
		CLASS_TYPE type {};

	public:

		explicit A(json& j) : type(CLASS_TYPE::A)
		{
			// Do something with "j".
		}

	};
	class B : public A
	{
		int Ranking {};

	protected:

		const int GetRank() const { return Ranking; }

	public:

		explicit B(json& j) : A(j)
		{
			type = CLASS_TYPE::B;

			// Do something with "j".
			Ranking = j["Rank"];
		}
	};
	class C : public A
	{
		vector<string> Data {};

	protected:

		void PrettyPrintData() const 
		{
			string str;
			for (auto& d : Data)
			{
				str += d;
				str += ',';
			}
			str.pop_back();
		}

	public:

		explicit C(json& j) : A(j)
		{
			type  = CLASS_TYPE::C;

			// Do something with "j".
			for (auto& d : j["Data"])
 				Data.push_back(nlohmann::to_string(d));
		}
	};
	//////////////////
	class DemoTest
	{
		json jf;

		vector<B> Bs;
		vector<C> Cs;

	public:
		////
		DemoTest(string fileName)
		{
			fstream ifs(fileName);
			jf = json::parse(ifs);
		}

		////
		const size_t Parse()
		{
			if (jf.is_array())
			{
				for (json& c : jf)
				{
					if (c["Type"] == "B")
						Bs.push_back(B(c));
					else if (c["Type"] == "C")
						Cs.push_back(C(c));
					else
						cerr << "Unknown class!" << c << endl;
				}
			}
			return Bs.size() + Cs.size();
		}
		////
		const vector<B> GetBs() const { return Bs; }
		const vector<C> GetCs() const { return Cs; }
	};
}
//////////////////
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
TEST_CASE("Testing Class Diagramming")
{
	Demo::DemoTest dt(DATA_FILE_PATH + "Test.json");
	
	// Invoke the parsing process
	CHECK(dt.Parse() == 2);
	CHECK(dt.GetBs().size() == 1);
	CHECK(dt.GetCs().size() == 1);

}

