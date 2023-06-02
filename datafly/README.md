# Ejecución

Instrucción de ejecución:

```
./datafly.out ../datasets/adult/age_categorical

Number of qids: 3
Enter qid name (0): age
Enter qid name (1): education
Enter qid name (2): native-country
Do yo want to anonymize preserving k-anonymity? [Y(y)/N(n)]: y
Insert K: 100
Suppresion Threshold (percentage, recommended 0.3-2%): 0
Do you want to use custom weights? (will only be used on gcp metric) [Y(y)/N(n)]: n
Do you want to treat some hierarchical attributes as numerical? (will only be used on metrics) [Y(y)/N(n)]: y
Enter number printed between brackets: 
	Age(0) Education(1) Native-country(2) 
		[enter q to quit] >> 0
	Education(0) Native-country(1) 
		[enter q to quit] >> q
```

Output:

```
===> Datafly Execution Time: 4 seconds
===> Number of clusters: 3
===> Writing data to file: 
	* generalized_tables/datasets/adult/age_categorical/100K.csv
===> Analysis: 
	* GCP: 0.802
	* DM: 446695226.000
	* CAvg: 100.540
	* GenILoss: 0.758
```