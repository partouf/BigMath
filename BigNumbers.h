
#ifndef _BIGNUMBERS_H
#define _BIGNUMBERS_H


#include <string>
#include <Atoms/GFFreeable.h>

// Array van bytes zijn Words (unsigned shorts),
//  aangezien 16bits * 16bits maximaal in 32bits resulteerd,
//  en we nogsteeds met 32bits cpu's werken


class TBigNumber : public TGFFreeable {
protected:
   unsigned int *FArrayI;

   bool FNegative;
   
   unsigned int iSizeArrayI;
   unsigned int iCountArrayI;

   void ClearArrayI();
   void ResizeArrayI( unsigned int iSize, bool bResizeMem );
   void CompressArrayI();
   
public:
   TBigNumber( const TBigNumber *aNumber );
   TBigNumber( int iNumber );
   TBigNumber();
   ~TBigNumber();
   
   
   void AssignValue( int iNumber );
   void AssignUnsigned( unsigned int iNumber );
   void AssignValue( const TBigNumber *aNumber );

   bool Equals( const TBigNumber *aNumber );
   bool GreaterThan( const TBigNumber *aNumber );
   bool LesserThan( const TBigNumber *aNumber );
   
   void Add( int iNumber );
   void Subtract( int iNumber );
   void Multiply( int iNumber );
   void Power( unsigned int iNumber );

   void Mod( int iNumber );
   void Mod( const TBigNumber *aNumber );

   void Add( const TBigNumber *aNumber );
   void Subtract( const TBigNumber *aNumber );
   void Multiply( const TBigNumber *aNumber );
   
   void Div2();
   void Mul2();
   
   bool IsZero();
   
   void Inc();
   void Dec();
   
   bool IsOdd();
   std::string ToString();
   std::string ToHexString();
   
   void LoadFromString( const char *s );
};





#endif // _BIGNUMBERS_H
