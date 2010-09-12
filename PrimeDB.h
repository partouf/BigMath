

#ifndef _PRIMEDB_H
#define _PRIMEDB_H


#include <Groundfloor/Molecules/GFVector.h>
#include "BigNumbers.h"

// TPrimeDB klasse
//  is vector met priemgetallen die later ook ingeladen moeten kunnen
//  worden vanuit een bestand cq. weggeschreven worden.
// moet eenvoudig en snel doorzoekbaar zijn,
// moet extra functies hebben tbv. opbouwen van nieuwe priemgetallen,
//  inclusief intelligentie om het opbouwen zo snel mogelijk te doen

class TPrimeDB : public TGFVector {
protected:
public:
   TBigNumber *GetNumber( unsigned int iIndex );

   void BuildBase();
   void AddFollowupPrime();
   bool ContainsDivider( const TBigNumber *subject );

   void LoadFromASCIIFile( const char *sFile );
};

#endif //_PRIMEDB_H
