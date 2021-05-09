/**
 * Laboratório 6 Computação concorrente
 *
 * Autor: Diego Costa Ferreira
 *
 * Descrição: Incrementa em 1 todos os elementos de um vetor usando n threads
 *
 * javac Main.java && java Main
 */

import java.util.Arrays;

class Worker implements Runnable {
    private final int[] arr;
    private final int id;
    private final int numThreads;

    public Worker(int[] arr, int id, int numThreads) {
        this.arr = arr;
        this.id = id;
        this.numThreads = numThreads;
    }

    @Override
    public void run() {
        final int blockSize = arr.length / this.numThreads;
        final int start = this.id * blockSize;
        final int end;
        if (id == numThreads - 1) {
            end = arr.length;
        } else {
            end = start + blockSize;
        }

        for (int i = start; i < end; ++i) {
            arr[i] += 1;
        }
    }
}


public class Main {
    private static final int ARRAY_SIZE = 10; // tamanho do array
    private static final int NUM_THREADS = 4; // número de threads

    public static void main(String[] args) {
        int[] arr = new int[ARRAY_SIZE];
        Thread[] threads = new Thread[NUM_THREADS];

        System.out.println(Arrays.toString(arr));

        for (int i = 0; i < NUM_THREADS; ++i) {
            threads[i] = new Thread(new Worker(arr, i, NUM_THREADS));
        }

        for (int i = 0; i < NUM_THREADS; ++i) {
            threads[i].start();
        }

        for (int i = 0; i < NUM_THREADS; ++i) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                System.err.println("Erro thread join: " + e.getMessage());
                return;
            }
        }

        System.out.println(Arrays.toString(arr));
    }
}
