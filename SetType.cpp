#include "SetType.h"
using namespace std;


template<class T>
SetType<T>::SetType() :  numBuckets(DEFAULT_BUCKETS), numElems(0), maxLoad(DEFAULT_LOAD_FACTOR){
    // Create an array of forward_lists and initially set to an empty forward_list
    buckets = new forward_list<T>[numBuckets];
}

template<class T>
SetType<T>::SetType(int numBucks) : numBuckets(numBucks), numElems (0), maxLoad(DEFAULT_LOAD_FACTOR) {
    buckets = new forward_list<T>[numBuckets];
}


template<class T>
SetType<T>::SetType(SetType &otherSet) {
    copySet(otherSet);           // call copySet - deep copy over all elements
}

template<class T>
void SetType<T>::copySet(const SetType &otherSet) {
    // de-allocate and re-allocate the table
    // iterate and copy elements of otherSet
    delete [] buckets;

    numBuckets = otherSet.numBuckets;
    numElems = otherSet.numElems;
    maxLoad = otherSet.maxLoad;
    this->buckets = new forward_list<T>[numBuckets];

    for (int i = 0; i < numBuckets; i++)
        for (auto it = otherSet.buckets[i].begin(); it != otherSet.buckets[i].end(); ++it)
            this->buckets[i].push_front(*it);
}


template<class T>
SetType<T>::~SetType() {
    delete [] buckets;
}


template<class T>
SetType<T> SetType<T>::operator+(T elem) {
    // does not change the original calling set
    SetType<T> result;
    result = SetType<T>(numBuckets);

    for (int i = 0; i < numBuckets; i++)
        for (auto it = buckets[i].begin(); it != buckets[i].end(); ++it)
            result.Add(*it);

    result.Add(elem);
    return result;
}

template<class T>
SetType<T> SetType<T>::operator-(T elem) {

    SetType<T> result;
    result = SetType<T>(numBuckets);

    for (int i = 0; i < numBuckets; i++)
        for (auto it = buckets[i].begin(); it != buckets[i].end(); ++it)
            result.Add(*it);

    result.Remove(elem);
    return result;
}

template<class T>
SetType<T> SetType<T>::operator+(SetType& otherSet) {       // union

    SetType<T> result;
    result.copySet(otherSet);

    for (int i = 0; i < numBuckets; i++)
        for (auto it = buckets[i].begin(); it != buckets[i].end(); ++it)
            if (!otherSet.Contains(*it))
                result.Add(*it);
    return result;
}


template<class T>
SetType<T> SetType<T>::operator*(SetType& otherSet) {    // intersection

    SetType<T> result;
    result = SetType<T>();

    for (int i = 0; i < numBuckets; i++)
        for (auto it = buckets[i].begin(); it != buckets[i].end(); ++it)
            if (otherSet.Contains(*it))
                result.Add(*it);
    return result;
}

template<class T>
SetType<T> SetType<T>::operator-(SetType& otherSet) {      // difference

    SetType<T> result;
    result = SetType<T>();

    for (int i = 0; i < numBuckets; i++)
        for (auto it = buckets[i].begin(); it != buckets[i].end(); ++it)
            if (!otherSet.Contains(*it))
                result.Add(*it);
    return result;
}

template<class T>
T SetType<T>::GetNextItem() {

    if (iterCount == numElems)
        throw IteratorOutOfBounds();

    while (bucketIter == buckets[currBucket].end()) {

        currBucket = (currBucket == numBuckets-1) ? 0 : currBucket + 1;
        bucketIter = buckets[currBucket].begin();
    }
    T item = *bucketIter;            // added: Returns the current item and then move to the next item

    bucketIter++;                    // bookkeeping via iterator variables
    iterCount++;
    return item;
}


template<class T>
int SetType<T>::GetHashIndex(const T& key) { // Done
    unordered_map<T,T> mapper;
    typename unordered_map<T,T>::hasher hashFunction = mapper.hash_function();
    return static_cast<int>(hashFunction(key) % numBuckets);
}


template<class T>
void SetType<T>::SetMaxLoad(double max) { // Done
    if (max < 0.1)
        maxLoad = 0.1;
    else
        maxLoad = max;
}

template<class T>
SetType<T>& SetType<T>::operator=(SetType const &other) {
    /* Copies contents of each bucket from otherSet into the buckets of this
     * and copy over the appropriate private variables; utilizing copySet function */
    copySet(other);
    return *this;
}


template<class T>
void SetType<T>::Add(const T &elem) {
    /* Add new element into set, if not already added. Keep track of bookkeeping variables. */

    if (Contains(elem))     return;

    if (LoadFactor() >= maxLoad)
        Rehash(2*numBuckets);

    int location = GetHashIndex(elem);

    while ( isOccupied(location) )
        location = (location + 41) % numBuckets;

    buckets[location].push_front(elem);
    numElems++;
}

template<class T>
void SetType<T>::Remove(T elem) {
    // Validate if element is within set, then remove it

    int location, start_location;
    bool moreToSearch = true;

    start_location = GetHashIndex(elem);
    location = start_location;

    do {
        for (auto it = buckets[location].begin(); it != buckets[location].end(); ++it) {
            if (*it == elem) {
                moreToSearch = false;
                break;
            }
            location = (location + 41) % numBuckets;
        }
    } while (location != start_location && moreToSearch);

    if (moreToSearch)
        return;

    buckets[location].remove(elem);
    --numElems;
}


template<class T>
bool SetType<T>::Contains(const T &_elem) {

    int location, start_location;

    start_location = GetHashIndex(_elem);
    location = start_location;

    do {
        for (auto it = buckets[location].begin(); it != buckets[location].end(); ++it)
            if (*it == _elem)
                return true;
        location = (location == numBuckets-1) ? 0 : location + 1;


    }while (location != start_location );

    return false;
}


template<class T>
bool SetType<T>::isOccupied(int newNumBucket) {
    for (auto it = buckets[newNumBucket].begin(); it != buckets[newNumBucket].end(); ++it)
        if (*it)
            return true;
    return false;
}


template<class T>
void SetType<T>::MakeEmpty() {

    for (int i = 0; i < numBuckets; i++)
        buckets[i].clear();
    numElems = 0;
    numBuckets = DEFAULT_BUCKETS;
}


template<class T>
double SetType<T>::LoadFactor() const {
    return static_cast<double>(numElems)/numBuckets;
}


template<class T>
void SetType<T>::ResetIterator() {
    // Initialize iterator to start bucket, plus resets currBucket and iterCount appropriately
    bucketIter = buckets[0].begin();
    currBucket = 0;
    iterCount = 0;
}


template<class T>
void SetType<T>::Rehash(int newNumBuckets) {
    // when loadfactor (number of elements / number of buckets) is greater than the maxload : i.e., 1.1 > 1.0
    // we want to rehash; dynamically allocate more space
    SetType<T> rehashedSet;
    rehashedSet = SetType<T>(newNumBuckets);           // copy over all elements to the new setType


    for ( int b = 0; b < this->numBuckets; b++) {     // For each bucket,

        // copy all the items from the bucket into the rehashedSet
        for ( auto it = buckets[b].begin(); it != buckets[b].end(); ++it)
            rehashedSet.Add(*it);
    }

    *this = rehashedSet;                               // omit delete because destructor will take care of it on stack
}


