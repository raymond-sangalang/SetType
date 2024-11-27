#ifndef SET_TYPE
#define SET_TYPE

#include<string>
#include<iostream>
#include<forward_list>

using namespace std;


class IteratorOutOfBounds {
    public:
        IteratorOutOfBounds() { printMsg(); }
        string Msg() {
            return "Iterator is out of bounds.  Nothing left to iterate over.";
        }
        void printMsg() {  std::cout << Msg(); }
};

const int DEFAULT_BUCKETS = 10;
const double DEFAULT_LOAD_FACTOR = 1.0;

template<class T>
class SetType
{
    public:
        // Constructors
        SetType();
        explicit SetType(int numBucks);
        SetType(SetType& other);
        // Destructor
        ~SetType();

        /* Function:     Adds elem to a bucket in the hash table
        * Postcondition: elem is stored in the array at position
        */
        void Add(const T &elem);

        /* Function:     Removes an elem from the table if it exists
        * Postcondition: The elem is not in the table
        */
        void Remove(T elem);

        /* Function:      Returns the elem in the map whose key-value pair matches
         *                  elem's key-value pair.
        * Precondition:   elem is initialized
        * Postcondition:  If there is an item located in elem's hashed set and values
        *                   are equal return true, otherwise false.
        */
        bool Contains(const T &elem);

        /* Function:       Returns occupancy of bucket
         * Precondition:   bucket is initialized
         * Postcondition:  If there is an item located in elem's hashed set
         *                  return true, otherwise false.                    */
        bool isOccupied(int newNumBucket);


        /* Function:      Initializes the set to an empty state.
        *  Postcondition: Set is empty.
        */
        void MakeEmpty();

        /* Function: Returns the number of items in the table.
        * Postcondition: Function value = number of items in the table.
        */
        int Size() const {  return numElems;  }

        /* Function:      Calculates the percent of buckets occupied
        *  Precondition:  list is initialized
        *  Postcondition: returns number of elements / number of buckets */
        double LoadFactor() const;

        /* Function:      Sets variable maxload of type double
        *  Postcondition: Sets ratio to acceptable load amount */
        void SetMaxLoad(double max);

        /* Function:      Resizes the buckets to accommodate more elements
        *  Precondition:  The set is initialized
        *  Postcondition: Number of buckets is doubled and all the elements
        *                 are added to adjusted table.                      */
        void Rehash(int newNumBuckets);



        // Operator Overload functions via operations of a Set

        /*  Function:      Adds elem to the set.
         *  Postcondition: If (the set is full and elem is not there), full
         *                  set exception is thrown; else elem is in the set only once */
        SetType operator+(T elem);

        /*  Function:      Removes an elem from the set if it is there.
         *  Postcondition: The elem is not in the set
         */
        SetType operator-(T elem);				      // Remove


        /* Function:      Takes the union of this->SetType and otherSet
        *  Postcondition: Returns the union of this->SetType and otherSet
         */
        SetType operator+(SetType& otherSet);

        /* Function:      Takes the Difference of this->SetType and otherSet
        *  Postcondition: Returns the Difference of this->SetType and otherSet
        */
        SetType operator-(SetType& otherSet);

        /* Function:      Takes the Intersection of this->SetType and otherSet
        *  Postcondition: Returns the Intersection of this->SetType and otherSet
        */
        SetType operator*(SetType& otherSet);

        /*  Function:  (deep) Copies otherSet to this instance of SetType
         *  Postcondition: this instance contains contents of each bucket from
         *                 otherSet into the buckets of this.
         */
        SetType& operator=(SetType const& otherSet);

        void ResetIterator();	// Reset iterator
        T GetNextItem();

    private:
        forward_list<T>* buckets;	// An array of forward_list's
                                    // (each index is a forward_list)
        int numBuckets;     // total number of buckets
        int numElems;       // total number of elements
        double maxLoad;     // load factor of the Set

        /*Function: Gets the hash index given the elem
        * @param key
        * @return The bucket
        * Postcondition: Returns an integer between 0 and numBuckets */
        int GetHashIndex(const T& key);



        // This function is used by the
        // copy constructor and the assignment operator.
        void copySet(const SetType& otherSet);

        // Iterator variables
        int currBucket;					// iterator value to track bucket
        int iterCount;                  // iterator value to track element

        // iterator of the current bucket
        mutable typename forward_list<T>::iterator bucketIter;

};

#include "SetType.cpp"
#endif