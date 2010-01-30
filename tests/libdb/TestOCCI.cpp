#include "Tests.h"
#include "occi.h"
#include <iostream>

using namespace oracle::occi;
using namespace std;

class clsTestOCCI {
private:
	void dropTable(Connection *conn) throw (SQLException) {
		cout << "dropTable()\n";

		Statement *stmt = conn->createStatement("DROP TABLE print_media");
		stmt->executeUpdate();
		conn->commit();
		conn->terminateStatement(stmt);
		
		cout << "~dropTable()\n";
	} 
	
	void createTable(Connection *conn) throw (SQLException) {
		cout << "createTable()\n";

		Statement *stmt = conn->createStatement("CREATE TABLE print_media(product_id NUMBER(6), ad_id NUMBER(6), ad_composite BLOB, ad_sourcetext CLOB)");
		stmt->executeUpdate();
		conn->commit();
		conn->terminateStatement(stmt);
		
		cout << "~createTable()\n";
	}

	void insertRows(Connection *conn) throw (SQLException) {
		cout << "insertRows()\n";

		Statement *stmt = conn->createStatement("INSERT INTO print_media(product_id, ad_id, ad_composite, ad_sourcetext) VALUES (6666, 11001,\'10001\',\'SHE\')");
		stmt->executeUpdate();
		stmt->setSQL("INSERT INTO print_media(product_id, ad_id, ad_composite, ad_sourcetext) VALUES (7777, 11001,\'1010\',\'HEM\')");
		stmt->executeUpdate();
		conn->commit();
		conn->terminateStatement (stmt);

		cout << "~insertRows()\n";
	}
	
	void deleteRows(Connection *conn) throw (SQLException) {

		cout << "deleteRows()\n";
/*
		Statement *stmt = conn->createStatement("DELЕTЕ FROM print_media");
		stmt->executeUpdate();
		cout << "deleteRows()\n";

		stmt->setSQL ("DELЕTЕ FROM print_media WHERE product_id = 7777");
		stmt->executeUpdate();
		conn->commit();
		conn->terminateStatement (stmt);
*/
		cout << "~deleteRows()\n";
	}

	/* populating the blob; */
	void populateBlob(Blob &blob, int size) throw (SQLException) {
		cout << "populateBlob()\n";

		Stream *outstream = blob.getStream (1,0);
		char *buffer = new char[size];
		memset (buffer, (char)10, size);
		outstream->writeBuffer (buffer, size);
		char *c = (char *)"";
		outstream->writeLastBuffer (c,0);
		delete (buffer);
		blob.closeStream (outstream);

		cout << "~populateBlob()\n";
	}
	
	/* printing the blob data as integer stream */
	void dumpBlob (Blob &blob, int size) throw (SQLException) {
		cout << "dumpBlob()\n";

		Stream *instream = blob.getStream (1,0);
		char *buffer = new char[size];
		memset (buffer, NULL, size);
		instream->readBuffer (buffer, size);
		cout << "dumping blob: ";
		for (int i = 0; i < size; ++i)
		cout << (int) buffer[i];
		cout << endl;
		delete (buffer);
		blob.closeStream (instream);

		cout << "~dumpBlob()\n";
	}

	/* public methods */
public:
	void runSample() throw (SQLException) {
		Environment *env = Environment::createEnvironment (Environment::DEFAULT);
		try {
			Connection *conn = env->createConnection ("system", "12345", "");
			createTable(conn);
			insertRows(conn);
			/* Reading a populated blob & printing its property. */
			string sqlQuery = "SELECT ad_composite FROM print_media WHERE product_id=6666";
			Statement *stmt = conn->createStatement (sqlQuery);
			ResultSet *rset = stmt->executeQuery ();
			while(rset->next()) {
				Blob blob = rset->getBlob(1);
				cout << "Opening the blob in Read only mode" << endl;
				blob.open(OCCI_LOB_READONLY);
				int blobLength = blob.length();
				cout << "Length of the blob is: " << blobLength << endl;
				dumpBlob(blob, blobLength);
				blob.close();
			}
			stmt->closeResultSet(rset);
			/*
			// Reading a populated blob & printing its property. 
			stmt->setSQL ("SELECT ad_composite FROM print_media WHERE product_id =7777 FOR UPDАTЕ");
			rset = stmt->executeQuery ();
			while(rset->next()) {
				Blob blob = rset->getBlob (1);
				cout << "Opening the blob in read write mode" << endl;
				blob.open(OCCI_LOB_READWRITE);
				cout << "Populating the blob" << endl;
				populateBlob(blob, 20);
				int blobLength = blob.length ();
				cout << "Length of the blob is: " << blobLength << endl;
				dumpBlob (blob, blobLength);
				blob.close ();
			}
			stmt->closeResultSet(rset); */
			deleteRows(conn);
			dropTable(conn);			
			conn->terminateStatement(stmt);
			env->terminateConnection(conn);
		} catch (SQLException ea) {
			cout << ea.what();
		}
		Environment::terminateEnvironment (env);
	}
}; //end of class

void testOCCI(bool isDropOldDB) {
	clsTestOCCI *o = new clsTestOCCI();
	o->runSample();
}
