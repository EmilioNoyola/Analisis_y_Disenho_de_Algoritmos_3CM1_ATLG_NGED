function CArray(numElements) {
    this.dataStore = [];
    this.pos = 0;
    this.numElements = numElements;
    this.insert = insert;
    this.toString = toString;
    this.clear = clear;
    this.setData = setData;
    this.setWorstCase = setWorstCase;
    this.swap = swap;
    this.selectionSort = selectionSort;

    for (var i = 0; i < numElements; ++i) {
        this.dataStore[i] = i;
    }
}

function setData() {
    for (var i = 0; i < this.numElements; ++i) {
        this.dataStore[i] = Math.floor(
            Math.random() * (this.numElements + 1)
        );
    }
}

function setWorstCase() {
    for (var i = 0; i < this.numElements; ++i) {
        this.dataStore[i] = this.numElements - i;
    }
}

function clear() {
    for (var i = 0; i < this.dataStore.length; ++i) {
        this.dataStore[i] = 0;
    }
}

function insert(element) {
    this.dataStore[this.pos++] = element;
}

function toString() {
    var retstr = "";

    for (var i = 0; i < this.dataStore.length; ++i) {
        retstr += this.dataStore[i] + " ";

        if (i > 0 && i % 10 == 0) {
            retstr += "\n";
        }
    }

    return retstr;
}

function swap(arr, index1, index2) {
    var temp = arr[index1];
    arr[index1] = arr[index2];
    arr[index2] = temp;
}

function selectionSort() {
    var min;

    for (var outer = 0; outer < this.dataStore.length - 1; ++outer) {
        min = outer;

        for (var inner = outer + 1; inner < this.dataStore.length; ++inner) {
            if (this.dataStore[inner] < this.dataStore[min]) {
                min = inner;
            }
        }

        if (min !== outer) {
            this.swap(this.dataStore, outer, min);
        }
    }
}

var elementos = [100, 500, 1500, 5000, 10000, 15000, 50000, 100000, 250000, 500000, 1000000];

console.log("Elementos,Tiempo_ms");

for (var i = 0; i < elementos.length; ++i) {
    var n = elementos[i];

    var arreglo = new CArray(n);

    arreglo.setWorstCase();

    const ini = performance.now();

    arreglo.selectionSort();

    const fin = performance.now();

    const tiempo = fin - ini;

    console.log(`${n},${tiempo}`);
}