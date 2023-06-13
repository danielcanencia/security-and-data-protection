
# Incognito

# Modelos de Privacidad

K-Anonymity, L-Diversity, T-Closeness.

# Ejecución

Instrucción de ejecución:

```
./incognito.out ../datasets/adult/age_categorical/

Number of qids: 3
Enter qid name (0): age
Enter qid name (1): education
Enter qid name (2): native-country
Confidential attributes: 
	Number of confidential attributes: 1
	Enter att name (0): occupation
Do yo want to anonymize preserving k-anonymity? [Y(y)/N(n)]: y
Insert K: 5
Do yo want to anonymize preserving l-diversity? [Y(y)/N(n)]: y
Insert L: 2
Do yo want to anonymize preserving t-closeness? [Y(y)/N(n)]: y
Insert T: 0.5
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
===> Incognito Execution Time: 138 seconds
===> Number of clusters: 16
===> Writing data to file: 
	* generalized_tables/datasets/adult/age_categorical/5K_2L_0.5000000T.csv
===> Analysis: 
	* GCP: 0.684
	* DM: 97697690.000
	* CAvg: 377.025
	* GenILoss: 0.684
```