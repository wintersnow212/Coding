#include <iostream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <string>
//#include <ctime>
//using namespace std;
#include <iostream>
#include <chrono>
#include <cmath>
#include <random>
#include <functional>
using namespace std;

// UniformRandom class
//
// CONSTRUCTION: with (a) no initializer or (b) a long
//     that specifies the initial state of the generator.
//     This random number generator is similar to 
//     drand48 in C/C++.
//
// ******************PUBLIC OPERATIONS*********************
//     Return a random number according to some distribution:
// int nextInt( )                          --> Uniform
// int nextDouble( )                       --> Uniform, [0 to 1)
// int nextInt( int high )                 --> Uniform [0..high)
// int nextInt( int low, int high )        --> Uniform [low..high)
// long nextLong( )                        --> Uniform

/**
 * Uniform Random Number generator class, using a 48-bit
 * linear congruential generator.
 */
class Random48
{
  private:
    static long long currentTimeMicroSeconds( )
    {
        auto now = chrono::high_resolution_clock::now( ).time_since_epoch( );       
        return (chrono::duration_cast<chrono::microseconds>( now ) ).count( );
    }
    
  public:
    /**
     * Construct this Random object with specified initial state
     */
    Random48( long long initialValue = currentTimeMicroSeconds( ) )
    {
        state = initialValue & MASK;
    }


    /**
     * Return a pseudorandom int, and change the
     * internal state.
     */
    int nextInt( )
    {
        return next( 32 );
    }

    /**
     * Return a pseudorandom int in range [0..high),
     * and change the internal state.
     */
    int nextInt( int high )
    {
        return static_cast<int>( abs( nextLong( ) % high ) ); 
    }
    
    /**
     * Return a pseudorandom double in the range [0..1)
     * and change the internal state.
     */
    double nextDouble( )
    {
        return ( ( static_cast<long long>( ( next( 26 ) ) ) << 27 ) + next( 27 ) )
                / static_cast<double>( 1LL << 53 );
    }

    /**
     * Return an int in the closed range [low,high], and
     * change the internal state.
     */
    int nextInt( int low, int high )
    {
        return nextInt( high - low + 1 ) + low;
    }

    /**
     * Return a 64-bit long, and change the internal state.
     * Note that all longs can be generated.
     */
    long long nextLong( )
    {
        return ( ( static_cast<long long>( next( 32 ) ) ) << 32 ) + next( 32 );
    }
    
  private:
    long long state;
      
    static const long long A = 25214903917LL;
    static const long long C = 11;
    static const long long M = ( 1LL << 48 );
    static const long long MASK = M - 1;

    /**
     * Return specified number of random bits
     */
    int next( int bits )
    {
        state = ( A * state + C ) & MASK;

        return state >> ( 48 - bits );
    }
};

// UniformRandom class
//
// CONSTRUCTION: with (a) no initializer or (b) a int
//     that specifies the initial state of the generator.
//     This random number generator uses the 32-bit Mersenne Twister
//
// ******************PUBLIC OPERATIONS*********************
//     Return a random number according to some distribution:
// int nextInt( )                          --> Uniform
// int nextDouble( )                       --> Uniform, [0 to 1)
// int nextInt( int high )                 --> Uniform [0..high)
// int nextInt( int low, int high )        --> Uniform [low..high)


static int currentTimeSeconds( )
{
    auto now = chrono::high_resolution_clock::now( ).time_since_epoch( );       
    return chrono::duration_cast<chrono::seconds>( now ).count( );
}
    
/**
 * Uniform Random Number generator class, using C++ Mersenne Twister.
 */
class UniformRandom
{
  public:
    UniformRandom( int seed = currentTimeSeconds( ) ) : generator{ seed }
    {
    }
      
    /**
     * Return a pseudorandom int.
     */
    int nextInt( )
    {
        static uniform_int_distribution<unsigned int> distribution;
        return distribution( generator );
    }

    /**
     * Return a pseudorandom int in range [0..high).
     */
    int nextInt( int high )
    {
        return nextInt( 0, high - 1 ); 
    }
    
    /**
     * Return a pseudorandom double in the range [0..1).
     */
    double nextDouble( )
    {
        static uniform_real_distribution<double> distribution( 0, 1 );
        return distribution( generator );
    }

    /**
     * Return an int in the closed range [low,high].
     */
    int nextInt( int low, int high )
    {
        uniform_int_distribution<int> distribution( low, high );
        return distribution( generator );
    }
    
  private:
    mt19937 generator;
};


template <typename AnyType>
class CuckooHashFamily
{
  public:
    size_t hash( const AnyType & x, int which ) const;
    int getNumberOfFunctions( );
    void generateNewFunctions( );
};

template <int count>
class StringHashFamily
{
  public:
    StringHashFamily( ) : MULTIPLIERS( count )
    {
        generateNewFunctions( );
    }
    
    int getNumberOfFunctions( ) const
    {
        return count;
    }
    
    void generateNewFunctions( )
    {
        for( auto & mult : MULTIPLIERS )
            mult = r.nextInt( );
    }
    
    size_t hash( const string & x, int which ) const
    {
        const int multiplier = MULTIPLIERS[ which ];
        size_t hashVal = 0;

        for( auto ch : x )
            hashVal = multiplier * hashVal + ch;
        
        return hashVal;
    }

  private:
    vector<int> MULTIPLIERS;
    UniformRandom r;
};



int nextPrime( int n );


#define MAX_LOAD 0.40  /* g++ 4.6 does not support static const double */

// CuckooHashing Hash table class
//
// CONSTRUCTION: an approximate initial size or default of 101
//
// ******************PUBLIC OPERATIONS*********************
// bool insert( x )       --> Insert x
// bool remove( x )       --> Remove x
// bool contains( x )     --> Return true if x is present
// void makeEmpty( )      --> Remove all items
// int hashCode( string str ) --> Global method to hash strings

template <typename AnyType, typename HashFamily>
class HashTable
{
  public:
    explicit HashTable( int size = 101 ) : array( nextPrime( size ) )
    {
        numHashFunctions = hashFunctions.getNumberOfFunctions( );
        rehashes = 0;
        makeEmpty( );
    }

    bool contains( const AnyType & x ) const
    {
        return findPos( x ) != -1;
    }

    void makeEmpty( )
    {
        currentSize = 0;
        for( auto & entry : array )
            entry.isActive = false;
    }

    bool insert( const AnyType & x )
    {
        if( contains( x ) ) 
            return false;
        
        if( currentSize >= array.size( ) * MAX_LOAD )
            expand( );
        
        return insertHelper1( x );
    }
    
    bool insert( AnyType && x )
    {
        if( contains( x ) ) 
            return false;
        
        if( currentSize >= array.size( ) * MAX_LOAD )
            expand( );
        
        return insertHelper1( std::move( x ) );
    }

    int size( ) const
    {
        return currentSize;
    }
    
    int capacity( ) const
    {
        return array.size( );
    }
    
    bool remove( const AnyType & x )
    {
        int currentPos = findPos( x );
        if( !isActive( currentPos ) )
            return false;

        array[ currentPos ].isActive = false;
        return true;
    }

  private:
      
    struct HashEntry
    {
        AnyType element;
        bool isActive;

        HashEntry( const AnyType & e = AnyType( ), bool a = false )
          : element{ e }, isActive{ a } { }    

        HashEntry( AnyType && e, bool a = false )
          : element{ std::move( e ) }, isActive{ a } { }
    };
    
    vector<HashEntry> array;
    int currentSize;
    int numHashFunctions;
    int rehashes;
    UniformRandom r;
    HashFamily hashFunctions;

  //  static const double MAX_LOAD = 0.40;  // Not supported in g++ 4.6
    static const int ALLOWED_REHASHES = 5;
    
    bool insertHelper1( const AnyType & xx )
    {
        const int COUNT_LIMIT = 100;
        AnyType x = xx;
        
        while( true )
        {
            int lastPos = -1;
            int pos;
            
            for( int count = 0; count < COUNT_LIMIT; ++count )
            {
                for( int i = 0; i < numHashFunctions; ++i )
                {
                    pos = myhash( x, i );
                    
                    if( !isActive( pos ) )
                    {
                        array[ pos ] = std::move( HashEntry{ std::move( x ), true } );
                        ++currentSize;
                        return true;
                    }
                }
                
                // None of the spots are available. Kick out random one
                int i = 0;
                do
                {
                    pos = myhash( x, r.nextInt( numHashFunctions ) );
                } while( pos == lastPos && i++ < 5 );
              
                lastPos = pos;
                std::swap( x, array[ pos ].element );
            }
            
            if( ++rehashes > ALLOWED_REHASHES )
            {
                expand( );     // Make the table bigger
                rehashes = 0;
            }
            else
                rehash( );
        }
    }
    
    bool insertHelper1( AnyType && x )
    {
        const int COUNT_LIMIT = 100;
        
        while( true )
        {
            int lastPos = -1;
            int pos;
            
            for( int count = 0; count < COUNT_LIMIT; ++count )
            {
                for( int i = 0; i < numHashFunctions; ++i )
                {
                    pos = myhash( x, i );
                    
                    if( !isActive( pos ) )
                    {
                        array[ pos ] = std::move( HashEntry{ std::move( x ), true } );
                        ++currentSize;
                        return true;
                    }
                }
                
                // None of the spots are available. Kick out random one
                int i = 0;
                do
                {
                    pos = myhash( x, r.nextInt( numHashFunctions ) );
                } while( pos == lastPos && i++ < 5 );
              
                lastPos = pos;
                std::swap( x, array[ pos ].element );
            }
            
            if( ++rehashes > ALLOWED_REHASHES )
            {
                expand( );     // Make the table bigger
                rehashes = 0;
            }
            else
                rehash( );
        }
    }

    bool isActive( int currentPos ) const
      {  return currentPos != -1 &&  array[ currentPos ].isActive; }

    // Method that search all hash function places
    int findPos( const AnyType & x ) const
    {
        for( int i = 0; i < numHashFunctions; ++i )
        {
            int pos = myhash( x, i );
            
            if( isActive( pos ) && array[ pos ].element == x )
                return pos;
        }

        return -1;
    }

    void expand( )
    {
        rehash( static_cast<int>( array.size( ) / MAX_LOAD ) );
    }
    
    void rehash( )
    {
        hashFunctions.generateNewFunctions( );
        rehash( array.size( ) );
    }
    
    void rehash( int newSize )
    {
        vector<HashEntry> oldArray = array;

            // Create new double-sized, empty table
        array.resize( nextPrime( newSize ) );
        for( auto & entry : array )
            entry.isActive = false;
        
            // Copy table over
        currentSize = 0;
        for( auto & entry : oldArray )
            if( entry.isActive )
                insert( std::move( entry.element ) );
    }
    
    
    size_t myhash( const AnyType & x, int which ) const
    {
        return hashFunctions.hash( x, which ) % array.size( );
    }
};


// Pre-c++11 style; not all compilers have new to_string function
template <typename Object>
string toString( Object x )
{
    ostringstream oss;
    oss << x;
    return oss.str( );
}


    // Simple main
int main( )
{
    const int NUMS = 2000000;
    const int GAP  =   37;
    const int ATTEMPTS = 1;
    int i;

    cout << "Checking... (no more output means success)" << endl;

    for( int att = 0; att < ATTEMPTS; ++att )
    { 
        cout << "ATTEMPT: " << att << endl;
        
        HashTable<string,StringHashFamily<3>> h1;
        HashTable<string,StringHashFamily<3>> h2;
        
        for( i = GAP; i != 0; i = ( i + GAP ) % NUMS )
        {
            if( !h1.insert( toString( i ) ) )
            {
                cout << "OOPS insert fails???!!!! " << i << endl;
            }
            
        }

        for( i = GAP; i != 0; i = ( i + GAP ) % NUMS )
            if( h1.insert( toString( i ) ) )
                cout << "INSERT OOPS!!! " << i << endl;
        
        h2 = h1;
        
        for( i = 1; i < NUMS; i += 2 )
            h2.remove( toString( i ) );
        
        for( i = 2; i < NUMS; i += 2 )
            if( !h2.contains( toString( i ) ) )
                cout << "Contains fails " << i << endl;
        
        for( i = 1; i < NUMS; i += 2 )
        {
            if( h2.contains( toString( i ) ) )
                cout << "CONTAINS OOPS!!! " <<  i << endl;
        }
        
        cout << "END OF ATTEMPT" << endl;
        
        if( h2.capacity( ) > NUMS * 4 )
            cout << "LARGE CAPACITY " << h2.capacity( ) << endl;
        
    }

    return 0;
}

/**
 * Internal method to test if a positive number is prime.
 * Not an efficient algorithm.
 */
bool isPrime( int n )
{
    if( n == 2 || n == 3 )
        return true;

    if( n == 1 || n % 2 == 0 )
        return false;

    for( int i = 3; i * i <= n; i += 2 )
        if( n % i == 0 )
            return false;

    return true;
}

/**
 * Internal method to return a prime number at least as large as n.
 * Assumes n > 0.
 */
int nextPrime( int n )
{
    if( n % 2 == 0 )
        ++n;

    for( ; !isPrime( n ); n += 2 )
        ;

    return n;
}

/**
 * A hash routine for string objects.
 */
int hashCode( const string & key )
{
    int hashVal = 0;

    for( auto ch : key )
        hashVal = 37 * hashVal + ch;

    return hashVal;
}

/**
 * A hash routine for ints.
 */
int hashCode( int key )
{
    return key;
}
