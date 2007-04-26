
#include "PrimeDB.h"

#include <iostream>


void TPrimeDB::BuildBase()
{
   TBigNumber *obj;

   // basis van priemgetallen database,
   //  het getal dat als enige geen oneven getal is
   //  en het getal dat met optelling van 2 tot een nieuw
   //   oneven getal dat wellicht een priem kan zijn
   
   // negeer 2, tijdverspilling voor AddFollowupPrime() functie
   //obj = new TBigNumber( 2 );
   //this->AddElement( obj );
   
   obj = new TBigNumber( 3 );
   this->AddElement( obj );
}

TBigNumber *TPrimeDB::GetNumber( unsigned int iIndex )
{
   TBigNumber *obj = NULL;
   
   if ( iIndex < this->Size() ) {

      obj = static_cast<TBigNumber *>( this->ElementAt( iIndex ) );

   }

   return obj;
}

void TPrimeDB::AddFollowupPrime()
{
   unsigned int iSize = this->Size();
   
   if ( iSize == 0 ) {
      this->BuildBase();
      
      iSize = this->Size();
   }
   
   
   // dit algorithme is op basis van de,
   //  aanname dat een oneven getal
   //  alleen op deling van andere priemen getest hoeft te worden
   //  om een nieuw priemgetal te produceren

   // tevens aanname dat de priemgetallen op volgorde staan
   //  van klein naar groot
   
   TBigNumber *copy = new TBigNumber( GetNumber(iSize - 1) );

   // 2 in aparte var, zodat deze niet telkens aangemaakt hoeft te worden
//   TBigNumber *two = new TBigNumber( 2 );

//   copy->Add( 2 );
   //copy->Add( two );
   
   // 2x INC aanroepen is zeer waarschijnlijk sneller
   copy->Inc();
   copy->Inc();
   
   // loop zolang er een divider in deze vector zit
   while ( this->ContainsDivider( copy ) ) {
//      copy->Add( 2 );
//      copy->Add( two );

      // 2x INC aanroepen is zeer waarschijnlijk sneller
      copy->Inc();
      copy->Inc();

   }
   
//   delete two;
   
   // uiteindelijk hebben we een nieuwe priem
   this->AddElement( copy );
}

bool TPrimeDB::ContainsDivider( const TBigNumber *subject )
{
   TBigNumber *copy = new TBigNumber( subject );
   TBigNumber *div;
   
   TBigNumber *halfSubj = new TBigNumber( subject );
   halfSubj->Div2();
   
   bool bRet = false;
   
   unsigned int c = this->Size() - 1;
   for ( unsigned int i = c; i >= 0; i-- ) {
      div = this->GetNumber( i );

      // hoeft alleen te vergelijken met getallen die lager dan de helft
      //  zijn van het onderwerp der divisie
      if ( !div->GreaterThan( halfSubj ) ) {
         copy->Mod( div );

         bRet = copy->IsZero();
         if ( bRet ) {
            break;
         }

         // terugzetten op juiste beginwaarde
         copy->AssignValue( subject );
      }
      
      if (i == 0) {
         // -1 bestaat niet bij unsigned int, dus nu al uit loop gaan
         break;
      }
   }
   
   delete halfSubj;
   delete copy;
      
   return bRet;
}

void TPrimeDB::LoadFromASCIIFile( const char *sFile ) {
   char sLine[1024];
   TBigNumber *aNum;
   
   FILE *fp;

   fp = fopen( sFile, "r" );
   
   while ( !feof( fp ) ) {
      fgets( sLine, 1024, fp );
      
      if ( strlen( sLine ) != 0 ) {
         aNum = new TBigNumber();
         this->AddElement( aNum );
         
         aNum->LoadFromString( sLine );
      }
   }
   
   fclose( fp );
}
