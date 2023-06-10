# K-Means

Algoritmos de agrupación basado en centroides, cuyo objetivo es dividir un conjunto de datos en K clases de equivalencia o clusters.

# Ejecución

Instrucción de ejecución:
```
./kmeans.out datasets/6-attributes/kmeans-10K.csv  16

Number of qids: 2
Enter qid name (0): weight
Enter qid name (1): ph
Epsilon (convergency threshold): 50
Do you want to use custom weights? (will only be used on gcp metric) [Y(y)/N(n)]: n
```

Output:

```
* K-Means algorithm is being run...

===> K-Means Execution Time: 0 seconds
===> Number of clusters: 16
* K-Means algorithm finished. A csv file will be generated....
* Writing output to directory: outputs/6-attributes/kmeans-10K_K16/clusters/
	* GCP: 0.851
	===> Evaluating K value to be used on DM and CAvg metrics based on clusters sizes
	===> K Value: 474
	* DM: 6403888.000
	* CAvg: 1.319
	* GenILoss: 0.851

```