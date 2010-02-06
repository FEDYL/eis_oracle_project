#include "Tests.h"
#include "occi.h"
#include <iostream>

using namespace oracle::occi;
using namespace std;

class clsTestOCCI {
private:
	void dropTable(Connection *conn) throw (SQLException) {
		PRINT_INFO_BEGIN("Drop table")
		Statement *stmt = conn->createStatement("DROP TABLE print_media");
		stmt->executeUpdate();
		conn->commit();
		conn->terminateStatement(stmt);
		PRINT_INFO_END
	} 
	
	void createTable(Connection *conn) throw (SQLException) {
		PRINT_INFO_BEGIN("Create table")
		Statement *stmt = conn->createStatement("CREATE TABLE print_media(product_id NUMBER(6), ad_id NUMBER(6), ad_composite BLOB, ad_sourcetext CLOB)");
		stmt->executeUpdate();
		conn->commit();
		conn->terminateStatement(stmt);
		PRINT_INFO_END
	}

	void insertRows(Connection *conn) throw (SQLException) {
		PRINT_INFO_BEGIN("Insert row")
		Statement *stmt = conn->createStatement("INSERT INTO print_media(product_id, ad_id, ad_composite, ad_sourcetext) VALUES (6666, 11001,\'10001\',\'SHE\')");
		stmt->executeUpdate();
		stmt->setSQL("INSERT INTO print_media(product_id, ad_id, ad_composite, ad_sourcetext) VALUES (7777, 11001,\'1010\',\'HEM\')");
		stmt->executeUpdate();
		conn->commit();
		conn->terminateStatement (stmt);
		PRINT_INFO_END
	}
	
	void deleteRows(Connection *conn) throw (SQLException) {
		PRINT_INFO_BEGIN("Delete row")
/*
		Statement *stmt = conn->createStatement("DELЕTЕ FROM print_media");
		stmt->executeUpdate();
		cout << "deleteRows()\n";

		stmt->setSQL ("DELЕTЕ FROM print_media WHERE product_id = 7777");
		stmt->executeUpdate();
		conn->commit();
		conn->terminateStatement (stmt);
*/
		PRINT_INFO_END
	}

	/* populating the blob; */
	void populateBlob(Blob &blob, int size) throw (SQLException) {
		PRINT_INFO_BEGIN("Populate BLOB")
		Stream *outstream = blob.getStream (1,0);
		char *buffer = new char[size];
		memset (buffer, (char)10, size);
		outstream->writeBuffer (buffer, size);
		char *c = (char *)"";
		outstream->writeLastBuffer (c,0);
		delete (buffer);
		blob.closeStream (outstream);
		PRINT_INFO_END
	}
	
	/* printing the blob data as integer stream */
	void dumpBlob (Blob &blob, int size) throw (SQLException) {
		PRINT_INFO_BEGIN("Dump BLOB")
		Stream *instream = blob.getStream (1,0);
		char *buffer = new char[size];
		memset (buffer, NULL, size);
		instream->readBuffer (buffer, size);
		PRINT_INFO_END
		PRINT_INFO("Dumping BLOB: ")
		for (int i = 0; i < size; ++i)
			cout << (int) buffer[i];
		cout << endl;
		delete (buffer);
		blob.closeStream (instream);
	}

public:
	void runSample() throw (SQLException) {
		Environment *env = Environment::createEnvironment (Environment::DEFAULT);
		try {
			Connection *conn = env->createConnection(USER, PASS, HOST);
			createTable(conn);
			insertRows(conn);
			string sqlQuery = "SELECT ad_composite FROM print_media WHERE product_id=6666";
			Statement *stmt = conn->createStatement (sqlQuery);
			ResultSet *rset = stmt->executeQuery ();
			while(rset->next()) {
				Blob blob = rset->getBlob(1);
				PRINT_INFO("BLOB in read only mode");
				blob.open(OCCI_LOB_READONLY);
				int blobLength = blob.length();
				cout << "Length of the BLOB is: " << blobLength << endl;
				dumpBlob(blob, blobLength);
				blob.close();
			}
			stmt->closeResultSet(rset);
			deleteRows(conn);
			dropTable(conn);			
			conn->terminateStatement(stmt);
			env->terminateConnection(conn);
		} catch (const SQLException &x) {
			std::cout << "no\nOops! " << x.what() << '\n' << std::endl;
		}
		Environment::terminateEnvironment (env);
	}
};

void testOCCI() {
	PRINT_INFO("\nTEST OCCI")
	clsTestOCCI *o = new clsTestOCCI();
	o->runSample();
	PRINT_INFO("\n")
}
