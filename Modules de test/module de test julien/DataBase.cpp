//**********************************************************************************************//
//* Programme	:	reader.cpp																	//
//*																	Date : 			26/04/2022, //		
//*																	Last update :   03/05/2022	//
//*---------------------------------------------------------------------------------------------//
//* Developper	:	julien laridant																//
//																								//
//**********************************************************************************************//

#include "DataBase.h"
#include "Reader.h"
#include "Reader.cpp"



void DataBase::AssociateCommand()
{reader = new Reader();
	
 

		reader->getresult();


}

void DataBase::BDD()
{
	QSqlDatabase Database;

	//Database = QSqlDatabase::addDatabase("QSQLITE");
	Database.setDatabaseName("./pharmacie.db");




	//simuler nombre 
	
	rand 3 % 5


	QSqlQuery query;
	if (query.exec("SELECT * FROM ordonnance"));
	
	{
		int code_barre  ; 

	}

}
