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
#include <qdebug.h>


int DataBase::AssociateCommand()
{	reader = new Reader();
	
 

	 reader->getresult();

		int nombre = 100614-001;

		if (nombre == reader->getresult() )
		{
			int aleatoire = rand() %3+1;
			return aleatoire;
		}   

		else
		{	
			qDebug() << "erreur medicament non commander";
		}




}

/*void DataBase::BDD()
{
	QSqlDatabase Database;

	//Database = QSqlDatabase::addDatabase("QSQLITE");
	Database.setDatabaseName("./pharmacie.db");




	//simuler nombre 
	
	
	 



	/*QSqlQuery query;
	if (query.exec("SELECT * FROM ordonnance"));
	
	{
		int code_barre  ; 

	}

}*/
