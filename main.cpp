#include <cstdlib>
#include <iostream>

#include "BigNumbers.h"

#include <cmath>

#include <Atoms/GFFreeable.h>
#include "PrimeDB.h"


using namespace std;


void testcase_optellingen()
{

   // testcase voor kruisoptelling 32bits max,
   //  helft van 32bits max is 7F is helft van FE

   unsigned int cpuAnswer;
   
   
   TBigNumber *a = new TBigNumber();
   TBigNumber *b = new TBigNumber();
   TBigNumber *c = new TBigNumber();
   TBigNumber *d = new TBigNumber();

   for ( unsigned int i = 0x1234FFFF; i < 0x7FFFFFFF; i++ ) {

      // note: wie zegt dat Assign() werkt?
      //  --> ook testen, maar hoe?
      a->AssignUnsigned( i );

      std::cout << "start: i = " << a->ToString() << endl;

      for ( unsigned int j = 0xFFFE; j < 0x7FFFFFFF; j++ ) {
         cpuAnswer = i + j;

         
         // lalala
         b->AssignUnsigned( j );
         
         // note2: wie zegt dat Add met integer werkt itt Add met TBigNumber* ?

         c->AssignValue( a );
         c->Add( b );

         // op de andere manier
         d->AssignUnsigned( cpuAnswer );
         
         // c->farray[0] == (bignumAnswer & 0x0000FFFF)
         // c->farray[1] == (bignumAnswer & 0xFFFF0000) >> 16
         if ( !c->Equals( d ) ) {
            std::cout << "fout: " << a->ToString() << " + " << b->ToString() << " != " << c->ToString() << std::endl;
            
            return;
         } else {
            std::cout << "goed: " << a->ToString() << " + " << b->ToString() << " == " << c->ToString() << std::endl;
         }
      }
      
   }
   
   delete d;
   delete c;
   delete b;
   delete a;
}

void mod_test()
{

   unsigned int cpuAnswer;
   unsigned int test;

   TBigNumber *a = new TBigNumber();
   TBigNumber *b = new TBigNumber();
   TBigNumber *c = new TBigNumber();
   TBigNumber *d = new TBigNumber();

   for ( unsigned int i = 0x0000FFFE; i < 0x1000F; i++ ) {

      // note: wie zegt dat Assign() werkt?
      //  --> ook testen, maar hoe?
      a->AssignUnsigned( i );

      std::cout << "start: i = " << a->ToString() << endl;


      for ( unsigned int j = 0xFFFF; j < 0x10000; j++ ) {
         cpuAnswer = i % j;

         // lalala
         b->AssignUnsigned( j );

         c->AssignValue( a );
         c->Mod( b );

         // op de andere manier
         d->AssignUnsigned( cpuAnswer );

         if ( !c->Equals( d ) ) {
            std::cout << "fout: " << a->ToString() << " mod " << b->ToString() << " != " << c->ToString() << std::endl;
            std::cout << "zou moeten zijn: " << d->ToString() << std::endl;
            
            //return;
         } else {
            std::cout << "goed: " << a->ToString() << " mod " << b->ToString() << " == " << c->ToString() << std::endl;
         }
      }


   }

   delete d;
   delete c;
   delete b;
   delete a;

}


void multiply_test()
{
   unsigned int cpuAnswer;

   TBigNumber *a = new TBigNumber();
   TBigNumber *b = new TBigNumber();
   TBigNumber *c = new TBigNumber();
   TBigNumber *d = new TBigNumber();

   for ( unsigned int i = 0x1111; i < 0xFFFE; i++ ) {

      // note: wie zegt dat Assign() werkt?
      //  --> ook testen, maar hoe?
      a->AssignUnsigned( i );

      std::cout << "start: i = " << a->ToString() << endl;


      for ( unsigned int j = 0x0; j < 0x0; j++ ) {
         cpuAnswer = i * j;

         // lalala
         b->AssignUnsigned( j );

         c->AssignValue( a );
         c->Multiply( b );

         // op de andere manier
         d->AssignUnsigned( cpuAnswer );

         if ( !c->Equals( d ) ) {
            std::cout << "fout: " << a->ToString() << " * " << b->ToString() << " != " << c->ToString() << std::endl;
            std::cout << "zou moeten zijn: " << d->ToString() << std::endl;

            return;
         } else {
            std::cout << "goed: " << a->ToString() << " * " << b->ToString() << " == " << c->ToString() << std::endl;
         }
      }


   }

   delete d;
   delete c;
   delete b;
   delete a;

}


void div2_test()
{
   unsigned int cpuAnswer;

   TBigNumber *a = new TBigNumber();
   TBigNumber *b = new TBigNumber();
   TBigNumber *c = new TBigNumber();
   TBigNumber *d = new TBigNumber();

   for ( unsigned int i = 0xFFFF; i < 0x1FFFF; i++ ) {

      // note: wie zegt dat Assign() werkt?
      //  --> ook testen, maar hoe?
      a->AssignUnsigned( i );

      std::cout << "start: i = " << a->ToString() << endl;

      cpuAnswer = i >> 1;

      // lalala
      b->AssignUnsigned( 2 );

      c->AssignValue( a );
      c->Div2();

      // op de andere manier
      d->AssignUnsigned( cpuAnswer );

      if ( !c->Equals( d ) ) {
         std::cout << "fout: " << a->ToString() << " / " << b->ToString() << " != " << c->ToString() << std::endl;
         std::cout << "zou moeten zijn: " << d->ToString() << std::endl;

         return;
      } else {
         std::cout << "goed: " << a->ToString() << " / " << b->ToString() << " == " << c->ToString() << std::endl;
      }

   }

   delete d;
   delete c;
   delete b;
   delete a;

}


void mul2_test()
{
   unsigned int cpuAnswer;

   TBigNumber *a = new TBigNumber();
   TBigNumber *b = new TBigNumber();
   TBigNumber *c = new TBigNumber();
   TBigNumber *d = new TBigNumber();

   for ( unsigned int i = 0x0; i < 0x7FFFFFFF; i++ ) {

      // note: wie zegt dat Assign() werkt?
      //  --> ook testen, maar hoe?
      a->AssignUnsigned( i );

      std::cout << "start: i = " << a->ToString() << endl;

      cpuAnswer = i << 1;

      // lalala
      b->AssignUnsigned( 2 );

      c->AssignValue( a );
      c->Mul2();

      // op de andere manier
      d->AssignUnsigned( cpuAnswer );

      if ( !c->Equals( d ) ) {
         std::cout << "fout: " << a->ToHexString() << " * " << b->ToHexString() << " != " << c->ToHexString() << std::endl;
         std::cout << "zou moeten zijn: " << d->ToString() << std::endl;

         return;
      } else {
         std::cout << "goed: " << a->ToHexString() << " * " << b->ToHexString() << " == " << c->ToHexString() << std::endl;
      }

   }

   delete d;
   delete c;
   delete b;
   delete a;

}


void mul2_test2()
{
   unsigned int cpuAnswer;

   TBigNumber *a = new TBigNumber();
   TBigNumber *b = new TBigNumber();
   TBigNumber *c = new TBigNumber();
   TBigNumber *d = new TBigNumber();

   a->AssignUnsigned( 1 );
   c->AssignValue( a );
   cpuAnswer = 1;
   for ( unsigned int i = 1; i < 33; i++ ) {
      std::cout << "start: i = " << a->ToString() << endl;

      cpuAnswer = cpuAnswer << 1;

      c->Multiply( 2 );
//      c->Mul2();

      // op de andere manier
      d->AssignUnsigned( cpuAnswer );

      if ( !c->Equals( d ) ) {
         std::cout << "fout: " << a->ToHexString() << " * " << b->ToHexString() << " != " << c->ToHexString() << std::endl;
         std::cout << "zou moeten zijn: " << d->ToString() << std::endl;

         return;
      } else {
         std::cout << "goed: " << a->ToHexString() << " * " << b->ToHexString() << " == " << c->ToHexString() << std::endl;
      }

   }

   delete d;
   delete c;
   delete b;
   delete a;
}

int main(int argc, char *argv[])
{
   TBigNumber *number = new TBigNumber( 0x1234ABCD );
   
   try {
/*
      std::cout << number->ToString() << std::endl;
      
//      number->Mod( 5 );
      
      //std::cout << number->ToString() << std::endl;
      
      TBigNumber *tmp = new TBigNumber( 0x1235ABCD );
      
      if ( tmp->Equals( number ) ) {
         std::cout << number->ToString() << " == " << tmp->ToString() << std::endl;
      } else {
         std::cout << number->ToString() << " != " << tmp->ToString() << std::endl;
      }

      if ( tmp->GreaterThan( number ) ) {
         std::cout << number->ToString() << " < " << tmp->ToString() << std::endl;
      } else {
         std::cout << number->ToString() << " > " << tmp->ToString() << std::endl;
      }
      
      delete tmp;


      number->AssignValue( 1 );
      std::cout << "getal = " << number->ToString() << std::endl;
      
      number->Multiply( 512437 );
      std::cout << "antwoord = " << number->ToString() << std::endl;


      number->Power( 2 );
//      std::cout << number->ToString() << std::endl;

      //system("pause");
      
      number->Mod( 10000 );
      std::cout << number->ToString() << std::endl;
*/
//      system("pause");
/*
      for ( unsigned int i = 0; i < 10; i++ ) {
         number->Subtract( 0x10000 );
         std::cout << number->ToString() << std::endl;
      }
*/

//      number->Add( 1 );
      
/*
      TBigNumber *tmp = new TBigNumber( 2 );

      for ( unsigned int i = 0; i < 64; i++ ) {
//         number->Inc();
         number->AssignValue(2);
         number->Power( i );
         
         if ( number->IsOdd() ) {
            std::cout << number->ToString() << "odd" << std::endl;

            //number->Multiply( tmp );
         } else {
            std::cout << number->ToString() << "even" << std::endl;
            
//            number->Add( 1 );
         }
      }

      delete tmp;
  */
     // number->Multiply( 0x10000 );

      
      
      

      //system("pause");
      /*
      unsigned int i = 0;
      while (i < 0xFFFFFFFF) {
         number->Add( 0xFFFF );
         
         number->Add( 0xFFFF0000 );
         
         i++;
         
         system("pause");
      }*/
      
   } catch (exception *e) {
      std::cout << e->what();
   }
   
   delete number;



   unsigned int iSize;
   TPrimeDB *db = new TPrimeDB();
   
   db->LoadFromASCIIFile( "primes.txt" );

   printf( "db read from file\n" );
   //system("pause");


   FILE *fp = fopen( "primes.txt", "a" );
//   fseek( fp, 0, 2 );   // seek eof
   
   printf( "seek eof done\n" );
   //system("pause");
/*
   for ( unsigned int i = 0; i < db->Size(); i++ ) {
      std::string x = db->GetNumber( i )->ToHexString();
      printf( "%s\n", x.c_str() );
      fprintf( fp, "%s\n", x.c_str() );
   }

*/

   for ( unsigned int i = 0; i < 0x7FFFFFFF; i++ ) {
      db->AddFollowupPrime();
      iSize = db->Size() - 1;
      
      std::string x = db->GetNumber( iSize )->ToHexString();
      printf( "%s\n", x.c_str() );
      fprintf( fp, "%s\n", x.c_str() );
      
      fflush( fp );
   }

   fprintf( fp, "\n" );

   fclose( fp );

   
   delete db;
   
   
/*
   testcase_optellingen();
   
   system("pause");
*/

    return EXIT_SUCCESS;
}
