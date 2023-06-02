# Ejecución

Instrucción de ejecución:

```
./mondrian.out ../datasets/adult/age_numerical/

Number of qids: 3
Enter qid name (0): age
Enter qid name (1): education
Enter qid name (2): native-country
Confidential attributes: 
	Number of confidential attributes: 0
Do yo want to anonymize preserving k-anonymity? [Y(y)/N(n)]: y
Insert K: 100
Do yo want to anonymize preserving l-diversity? [Y(y)/N(n)]: n
Do yo want to anonymize preserving t-closeness? [Y(y)/N(n)]: n
Do you want to use custom weights? (will only be used on gcp metric) [Y(y)/N(n)]: n
Do you want to treat some hierarchical attributes as numerical? (will only be used on metrics) [Y(y)/N(n)]: n
```

Output:

```
===> Mondrian Execution Time: 6 seconds
===> Number of clusters: 47
===> Writing data to file: 
	* generalized_tables/datasets/adult/age_numerical/100K.csv
===> Analysis: 
	* GCP: 0.669
	* DM: 27069192.000
	* CAvg: 6.417
	* GenILoss: 0.669
```