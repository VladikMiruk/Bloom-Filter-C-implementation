
#include <stdlib.h>

#include "bloomfilter.h"

//Creazione di un nuovo Bloom filter
BloomFilter createBloomFilter(int k, int m)
{
	BloomFilter bloomfilter;
	int i;

	bloomfilter.filter = malloc(m * sizeof(bool)); //Creo il Bloom filter come array di bool e metto tutti i valori su false
	for (i=0 ; i < m ; i++)
	{
		bloomfilter.filter[i] = false;
	}

	bloomfilter.k = k;
	bloomfilter.m = m;
	bloomfilter.hashSeeds = malloc(10*sizeof(int));//Creo un array di interi dove memorizare i seed per le funzioni hash e lo inizializzo con i valori da 1 a 10

	for (i = 0; i < k; i++)
		bloomfilter.hashSeeds[i] = i+1;

	return bloomfilter;
}

// Libera tutta la memoria allocata per il bloom filter
void freeBloomFilter(BloomFilter bloomFilter)
{
	free(bloomFilter.filter);  //libero la memoria allocata per il filtro e per i hash seed
	free(bloomFilter.hashSeeds);

	return;
}

//Inserimento di un elemento all'interno del bloom filter
void bfInsertion(BloomFilter* bloomFilter,const char* elem,int elemLen)
{
	int pos;

	for (int i = 0; i < bloomFilter->k; i++) //chiamo la funzione hash 10 volte usando i 10 seed diversi
	{
		pos = hashFunction(elem, elemLen, bloomFilter->hashSeeds[i], bloomFilter->m);
		bloomFilter->filter[pos] = true; //uso il risultato della funzione hash per determinare la posizione del filtro in cui settare il valore su true
	}

	return;
}

//Inserimento di un dataset di elementi all'interno del bloom filter
void bfConstruction(BloomFilter* bloomFilter,const char** dataset,const int* datasetElemLen,int n)
{
	for (int i = 0; i < n; i++) //Uso la funzione bfInsertion per ogni elemento contenuto nel dataset
	{
		bfInsertion(bloomFilter, dataset[i], datasetElemLen[i]);
	}

	return;
}

//Ricerca di un elemento all'interno del bloom filter
bool bfSearch(BloomFilter bloomFilter,const char* elem,int elemLen)
{
	int  pos;
	for (int i = 0 ; i < bloomFilter.k ; i++)//chiamo la funzione hash 10 volte come nella funzione bfInsertion
	{
		pos = hashFunction(elem, elemLen, bloomFilter.hashSeeds[i], bloomFilter.m);
		if (bloomFilter.filter[pos] == false) //uso il risultato della funzione hash per determinare la posizione da controllare nel filtro
			return false; //nel caso anche solo una delle posizioni controllate restituisca false la funzione di ricerca restituisce il valore false
	}
	return true; //se dopo aver calcolato tutte e 10 le funzioni hash i valori controllati erano sempre true allora la funzione di ricerca restituisce true
}

//Conteggio del numero di elementi di un dataset presenti nel bloom filter
int countDatasetMembership(BloomFilter bloomFilter, const char** dataset, const int* datasetElemLen, int n)
{
	int count=0;//creo il contatore count che indichera il numero di elementi che sono presenti nella lista
	

	for (int i = 0; i < n; i++)//Chiamo la funzione bfSearch per ogni elemento del dataset e ogni volta che la funzione di ricerca restituisce true incremento il contatore count di uno
	{	
		if (bfSearch(bloomFilter, dataset[i], datasetElemLen[i])==true)
			count++;
	}

	return count;
}