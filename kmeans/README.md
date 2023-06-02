# K-Means

Algoritmos de agrupación basado en centroides, cuyo objetivo es dividir un conjunto de datos en K clases de equivalencia o clusters.

# Ejecución

Instrucción de ejecución:
```
./kmeans.out datasets/3-attributes/k-means-100.csv 2

Number of qids: 2
Enter qid name (0): weight
Enter qid name (1): ph
Do you want to use custom weights? (will only be used on gcp metric) [Y(y)/N(n)]: n
```

Output:

```
* K-Means algorithm is being run...

===> K-Means Execution Time: 1483 microseconds
===> Number of clusters: 2
* K-Means algorithm finished. A csv file will be generated....
* Writing output to directory: outputs/3-attributes/k-means-100_K2/clusters/
	* GCP: 0.969
	===> Evaluating K value to be used on DM and CAvg metrics based on clusters sizes
	===> K Value: 39
	* DM: 5242.000
	* CAvg: 1.282
	* GenILoss: 0.969

```