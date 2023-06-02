# K-Member

Algoritmos de agrupación cuyo objetivo es dividir un conjunto de datos en clases de equivalencia o clusters, en el cada uno de llos contenga exactamente K registros o más.

# Ejecución

Instrucción de ejecución tratando el atributo **age** como categórico:

```
./kmember.out dataset/adult-800-age-categorical

Number of qids: 3
Enter qid name (0): age
Enter qid name (1): education
Enter qid name (2): native-country
Do yo want to use a confidential attribute? (will only be used if l-diversity is choosen) [Y(y)/N(n)]: y
Confidential attribute: 
	Enter att name: occupation
Do yo want to anonymize preserving k-anonymity? [Y(y)/N(n)]: y
Insert K: 4
Do yo want to anonymize preserving l-diversity? [Y(y)/N(n)]: y
Insert L: 2
Diversity Penalty: 1
Diversity Metric:
	Equal Diversity Metric (0), Sensitive Diversity Metric (1): 0
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
===> Kmember Execution Time: 966 seconds
===> Number of clusters: 200
===> Writing data
===> Writing data to file: 
	* generalized_tables/dataset/adult-800-age-categorical/4K_2L.csv
	* GCP: 0.175
	* DM: 3200.000
	* CAvg: 1.000
	* GenILoss: 0.167

```