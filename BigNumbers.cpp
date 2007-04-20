
#include <cstdlib>
#include <iostream>
#include <string>

#include "BigNumbers.h"

// macro max() versie is sneller dan de template
#define quickmax(x,y) ((x) > (y) ? (x) : (y))
#define quickmin(x,y) ((x) < (y) ? (x) : (y))


void TBigNumber::ClearArrayI() {
   memset( FArrayI, 0, iSizeArrayI );
}

void TBigNumber::ResizeArrayI( unsigned int iSize, bool bResizeMem ) {
   unsigned  int *TmpArrayI;
   unsigned int oversize = 0x80000000;

   if ((iSize & oversize) != 0) {
      throw new std::exception();
   }

   unsigned int iAllocSize = iSize << 2;

   if ( bResizeMem || ( iAllocSize > iSizeArrayI ) ) {
      TmpArrayI = static_cast<unsigned int *>( malloc( iAllocSize ) );

      // init met nullen
      memset( TmpArrayI, 0, iAllocSize );

      // min omdat de nieuwe array ook kleiner kan zijn, en dan niet alles
      //  gekopieerd mag worden ivm buffer overflowing
      unsigned int iMinSize = quickmin( iSizeArrayI, iAllocSize );

      if ( FArrayI != NULL ) {
         memcpy( TmpArrayI, FArrayI, iMinSize );

         free( FArrayI );
      }

      // pointer is veranderd
      FArrayI = TmpArrayI;

      // gealloceerde lengte is veranderd
      iCountArrayI   = iSize;
      iSizeArrayI    = iAllocSize;
   } else {
      // alleen nieuwe count invullen, we zijn groot genoeg
      iCountArrayI   = iSize;
   }
}

void TBigNumber::CompressArrayI() {
   unsigned int i;
   unsigned int c = iCountArrayI - 1;

   for ( i = c; i > 0; i-- ) {
      if ( FArrayI[i] != 0 ) {
         break;
      }
   }

   if (i != c)
   {
      this->ResizeArrayI( i + 1, false );
   }
}

TBigNumber::TBigNumber() {
   FArrayI        = NULL;
   iCountArrayI   = 0;
   iSizeArrayI    = 0;
   FNegative      = false;
}

TBigNumber::TBigNumber( const TBigNumber *aNumber ) {
   FArrayI        = NULL;
   iCountArrayI   = 0;
   iSizeArrayI    = 0;
   FNegative      = false;

   AssignValue( aNumber );
}

TBigNumber::TBigNumber( int iNumber ) {
   // inits
   FArrayI        = NULL;
   iCountArrayI   = 0;
   iSizeArrayI    = 0;
   FNegative      = false;

   AssignValue( iNumber );
}

TBigNumber::~TBigNumber() {
   if ( FArrayI != NULL ) {
      free( FArrayI );
   }
}

void TBigNumber::AssignValue( int iNumber ) {
   if ( FArrayI != NULL ) {
      ClearArrayI();
   }
   
   FNegative = (iNumber < 0);
   
   unsigned int iTmp;
   if ( FNegative ) {
      iTmp = iNumber * -1;
   } else {
      iTmp = iNumber;
   }
   
   this->ResizeArrayI( 1, false );
   this->FArrayI[0] = iTmp;
/*
   unsigned int hi = (iTmp & 0xFFFF0000);
   if ( hi != 0 ) {
      this->ResizeArrayI( 2, false );
      this->FArrayI[0] = static_cast<unsigned short int>(iTmp & 0x0000FFFF);
      this->FArrayI[1] = static_cast<unsigned short int>(hi >> 16);
   } else {
      this->ResizeArrayI( 1, false );
      this->FArrayI[0] = static_cast<unsigned short int>(iTmp & 0x0000FFFF);
   }
*/
}

void TBigNumber::AssignUnsigned( unsigned int iNumber ) {
   if ( FArrayI != NULL ) {
      ClearArrayI();
   }

   this->ResizeArrayI( 1, false );
   this->FArrayI[0] = iNumber;
/*
   unsigned int hi = (iNumber & 0xFFFF0000);
   if ( hi != 0 ) {
      this->ResizeArrayI( 2, false );
      this->FArrayI[0] = static_cast<unsigned short int>(iNumber & 0x0000FFFF);
      this->FArrayI[1] = static_cast<unsigned short int>(hi >> 16);
   } else {
      this->ResizeArrayI( 1, false );
      this->FArrayI[0] = static_cast<unsigned short int>(iNumber & 0x0000FFFF);
   }
*/
}

void TBigNumber::AssignValue( const TBigNumber *aNumber ) {
   if ( FArrayI != NULL ) {
      if ( this->iSizeArrayI < aNumber->iSizeArrayI ) {
         free( FArrayI );
         
         FArrayI = static_cast<unsigned int *>( malloc( aNumber->iSizeArrayI ) );
      } else {
         ClearArrayI();
      }
   } else {
      this->ResizeArrayI( aNumber->iCountArrayI, false );
   }
   
   if ( FArrayI != NULL ) {
      memcpy( FArrayI, aNumber->FArrayI, aNumber->iSizeArrayI );
      

      // gealloceerde lengte is veranderd
      iCountArrayI   = aNumber->iCountArrayI;
      iSizeArrayI    = aNumber->iSizeArrayI;
      FNegative      = aNumber->FNegative;
   } else {
      // niet genoeg geheugen?
   }
}


void TBigNumber::Add( const TBigNumber *aNumber ) {
   unsigned int iLen = quickmax( this->iCountArrayI, aNumber->iCountArrayI ) + 1;

   if ( this->iCountArrayI != iLen ) {
      iLen += 2;

      this->ResizeArrayI( iLen, false );
   }

   unsigned short int remA;
   unsigned int resA;
   unsigned int ref;
   unsigned char add = 0;
   unsigned int resB = 0;

   unsigned int i;
   for ( i = 0; i < iLen; i++ ) {
      if ( i < aNumber->iCountArrayI ) {
         remA = aNumber->FArrayI[i];
      } else {
         remA = 0;
      }

      resA = this->FArrayI[i];
      
      ref = resA;
      resA += remA;
      if ( resA < ref ) {     // langzame overflow detectie
         add++;
      }
      
      ref = resA;
      resA += resB;
      if ( resA < ref ) {     // langzame overflow detectie
         add++;                  // volgens mij kom je hier nooit...
      }

      resB = add;

      this->FArrayI[i] = resA;
   }
   
   this->CompressArrayI();
}

void TBigNumber::Subtract( const TBigNumber *aNumber ) {
   unsigned int iLen = quickmax( this->iCountArrayI, aNumber->iCountArrayI );
   this->ResizeArrayI( iLen, false );

   unsigned short int remA;
   unsigned short int remB = 0;
   unsigned int resA;
   unsigned int resB;
   unsigned int remC = 0;
   

   // Note: werkt niet met negatieve mutaties
   
   /*
   
   waarschijnlijk op te lossen door extra boolean in subtract en add functies,
   
    waarbij aftrekking voor <0 getallen afgehandeld worden door de Add functie
    waarbij geen/reversed rekening gehouden wordt met FNegative.
    bwzv: -a - b == - ( a + b )

    waarbij optelling voor <0 getallen afgehandeld worden door de Subtract functie
    waarbij geen/reversed rekening gehouden wordt met FNegative.
    bwzv: -a + b == - ( a - b )

    waarbij aftrekking voor > 0 getallen normaal worden afgehandeld t/m 0.
    bwzv: a - b == a - b
    
    waarbij aftrekking met < 0 getallen voor > 0 getallen afgehandeld worden door
    de Add functie waarbij wel rekening gehouden wordt met FNegative,
    bwzv: a - -b == a + b
    
    waarbij aftrekking met < 0 getallen voor < 0 getallen afgehandeld worden door
    de Add functie waarbij wel rekening gehouden wordt met FNegative.
    bwzv: -a - -b = -a + b
    
    matrix mutaties?
   
   */

   unsigned int i = 0;
   for ( i = 0; i < iLen; i++ ) {
      if ( i > aNumber->iCountArrayI ) {
         remA = 0;
      } else {
         remA = aNumber->FArrayI[i];
      }

      if ( i > this->iCountArrayI ) {
         resA = 0;
      } else {
         resA = this->FArrayI[i];
      }

      resB = remA + remC;
      resA = resA - resB;

      remA = static_cast<unsigned short int>(resA & 0x0000FFFF);
      remB = static_cast<unsigned short int>((resA & 0xFFFF0000) >> 16);
      
      if (remB != 0) {
         remC = remB + 2;
      } else {
         remC = 0;
      }
      
      this->FArrayI[i] = remA;
   }
   
   this->CompressArrayI();
}

void TBigNumber::Add( int iNumber ) {
   TBigNumber *tmp = new TBigNumber( iNumber );

   this->Add( tmp );

   delete tmp;
}

void TBigNumber::Subtract( int iNumber ) {
   TBigNumber *tmp = new TBigNumber( iNumber );

   this->Subtract( tmp );

   delete tmp;
}

void TBigNumber::Multiply( int iNumber ) {
   TBigNumber *tmp = new TBigNumber( iNumber );
   
   this->Multiply( tmp );
   
   delete tmp;
}

void TBigNumber::Mod( int iNumber ) {
   TBigNumber *tmpRegister = new TBigNumber( iNumber );

   this->Mod( tmpRegister );
   
   delete tmpRegister;
}


void TBigNumber::Mod( const TBigNumber *aNumber ) {
// TBigNumber *tmpRegister = new TBigNumber( 0 );
   // eerste keer is het toch altijd groter dan 0...
   TBigNumber *tmpRegister = new TBigNumber( aNumber );
   TBigNumber *tmpSub = new TBigNumber( aNumber );

   // a mod b = (b - (sum(b) - a)) = antwoord

   while ( tmpRegister->LesserThan( this ) ) {
      tmpRegister->Add( aNumber );
   }
   
   tmpRegister->Subtract( this );

   tmpSub->Subtract( tmpRegister );
   
   // uitzondering; als antwoord gelijk is aan b, dan is antwoord 0
   if ( tmpSub->Equals( aNumber ) ) {
      this->AssignValue( 0 );
   } else {
      this->AssignValue( tmpSub );
   }

   delete tmpSub;
   delete tmpRegister;
}


void TBigNumber::Multiply( const TBigNumber *aNumber ) {
   TBigNumber *Number1 = new TBigNumber();
   TBigNumber *Number2 = new TBigNumber();
   
   TBigNumber *tmpRegister = new TBigNumber( 0 );
   TBigNumber *nCount = new TBigNumber( 0 );
   
   if ( this->GreaterThan( aNumber ) ) {
      Number1->AssignValue( aNumber );
      Number2->AssignValue( this );
   } else {
      Number1->AssignValue( this );
      Number2->AssignValue( aNumber );
   }

   // zo min mogelijk proberen te loopen
   while ( nCount->LesserThan( Number1 ) ) {
      tmpRegister->Add( Number2 );

      nCount->Inc();
   }

   this->AssignValue( tmpRegister );
   
   delete tmpRegister;
   delete nCount;
   
   delete Number2;
   delete Number1;
}

void TBigNumber::Power( unsigned int iNumber ) {
   TBigNumber *tmpRegister = new TBigNumber( this );

   for ( unsigned int i = 1; i < iNumber; i++ ) {
      tmpRegister->Multiply( this );
   }
   
   if (iNumber == 0) {
      this->AssignValue( 1 );
   } else {
      this->AssignValue( tmpRegister );
   }

   delete tmpRegister;
}

void TBigNumber::Inc() {
   bool b = false;
   
   if ( iCountArrayI > 0 ) {
      b = ( FArrayI[0] < 0xFFFFFFFF );
   }
   
   if ( b ) {
      FArrayI[0]++;
   } else {
      this->Add( 1 );
   }
}

void TBigNumber::Dec() {
   bool b = false;

   if ( iCountArrayI > 0 ) {
      b = ( FArrayI[0] > 0 );
   }

   if ( b ) {
      FArrayI[0]--;
   } else {
      this->Subtract( 1 );
   }
}

bool TBigNumber::IsZero() {
   if ( this->iCountArrayI == 1 ) {
      return ( this->FArrayI[0] == 0 );
   } else {
      return false;
   }
}

bool TBigNumber::Equals( const TBigNumber *aNumber ) {
   if ( this->iCountArrayI != aNumber->iCountArrayI ) {
      return false;
   }

   for ( unsigned int i = 0; i < this->iCountArrayI; i++ ) {
      if ( FArrayI[i] != aNumber->FArrayI[i] ) {
         return false;
      }
   }

   return true;
}

bool TBigNumber::GreaterThan( const TBigNumber *aNumber ) {
   if ( this->iCountArrayI == aNumber->iCountArrayI ) {

      for ( unsigned int i = this->iCountArrayI - 1; i > 0; i-- ) {
         if ( this->FArrayI[i] != aNumber->FArrayI[i] ) {
            return ( this->FArrayI[i] > aNumber->FArrayI[i] );
         }
      }

      return ( this->FArrayI[0] > aNumber->FArrayI[0] );

   } else {
      return (this->iCountArrayI > aNumber->iCountArrayI);
   }
   
   return false;
}

bool TBigNumber::LesserThan( const TBigNumber *aNumber ) {
   if ( this->iCountArrayI == aNumber->iCountArrayI ) {

      for ( unsigned int i = this->iCountArrayI - 1; i > 0; i-- ) {
         if ( this->FArrayI[i] != aNumber->FArrayI[i] ) {
            return ( this->FArrayI[i] < aNumber->FArrayI[i] );
         }
      }

      return ( this->FArrayI[0] < aNumber->FArrayI[0] );

   } else {
      return (this->iCountArrayI < aNumber->iCountArrayI);
   }

   return false;
}

bool TBigNumber::IsOdd() {
   bool res = false;
   
   if ( iCountArrayI > 0 ) {
      res = ( (FArrayI[0] & 1) != 0 );
   }
   
   return res;
}

void TBigNumber::Div2() {
   unsigned int i;
   bool bOne = false;

   for ( i = 0; i < this->iCountArrayI; i++ ) {
      bOne = (FArrayI[i] & 1);
      
      FArrayI[i] = FArrayI[i] >> 1;

      if (bOne && (i > 0)) {
         FArrayI[i-1] = (FArrayI[i-1] | 0x80000000);
      }
   }
   
   this->CompressArrayI();
}

void TBigNumber::Mul2() {
   unsigned int i;
   unsigned int c;
   bool bOne = false;

   c = this->iCountArrayI;
   if (c != 0 ) {
      bOne = ((FArrayI[c - 1] & 0x80000000) != 0);
      
      if (bOne) {
         this->ResizeArrayI( c + 1, true );

         // c niet ophogen, zodat we dit niet dubbel hoeven te doen
      } else {
         c--;
      }
   } else {
      return;
   }

   for ( i = c; i >= 0; i-- ) {
      bOne = ((FArrayI[i] & 0x80000000) != 0);

      FArrayI[i] = FArrayI[i] << 1;
      if ( bOne ) {
         FArrayI[i+1] = (FArrayI[i+1] | 0x00000001);
      }

      if ( i == 0 ) {
         break;
      }
   }
}

std::string TBigNumber::ToString() {
   std::string sRes = "";
   char tmp[1024];
   
   unsigned int i;
   unsigned int c = this->iCountArrayI;
   unsigned int d = c - 1;

   if (FNegative) {
      sRes = sRes + "- ";
   }

   for ( i = 0; i < c; i++ ) {
      sprintf( &tmp[0], "%d", FArrayI[i] );

      if ( i < d ) {
         sRes = sRes + tmp + ",";
      } else {
         sRes = sRes + tmp;
      }
   }

   return sRes;
}

std::string TBigNumber::ToHexString()
{
   std::string sRes = "";
   char tmp[1024];

   unsigned int i;
   unsigned int c = this->iCountArrayI;
   unsigned int d = c - 1;

   for ( i = 0; i < c; i++ ) {
      sprintf( &tmp[0], "%08x", FArrayI[i] );
      
      // NIET omgekeerd teruggeven
      sRes = sRes + tmp;
   }

   return sRes;
}

unsigned char getHexVal( unsigned char c ) {
   if ( (c >= '0') && (c <= '9') ) {
      return (c - '0');
   } else {
      return (c - 'a') + 10;
   }
}

// eigenlijk kan dit niet, de string kan langer zijn dan een unsigned int
void TBigNumber::LoadFromString( const char *s ) {
   unsigned short c1,c2,c3,c4 = 0;
   unsigned int i = 0;
   unsigned int j = 0;
   unsigned int count = strlen(s);

   count = count >> 2;
   
   ResizeArrayI( count, false );

   for ( i = 0; i < count; i++ ) {
      j = i << 2;
      c4 = getHexVal( s[j + 0] );
      c3 = getHexVal( s[j + 1] );
      c2 = getHexVal( s[j + 2] );
      c1 = getHexVal( s[j + 3] );

      FArrayI[i] = FArrayI[i] | (c4 << 12);
      FArrayI[i] = FArrayI[i] | (c3 << 8);
      FArrayI[i] = FArrayI[i] | (c2 << 4);
      FArrayI[i] = FArrayI[i] | (c1);
   }
}
