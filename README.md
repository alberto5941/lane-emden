# Lane-Emden

Integrazione numerica dell'equazione di Lane-Emden con il metodo di
Runge-Kutta del quarto ordine (RK4), per modellare il Sole come stella
politropica.

Si pone $\rho(r) = \rho_C \theta^n(\xi)$ e $r = a\xi.$
Si ottiene l'equazione di Lane-Emden:
$\frac{d^2\theta}{d\xi^2} = -\frac{2}{\xi}\frac{d\theta}{d\xi} - \theta^n,
\qquad \theta(0) = 1, \quad \theta'(0) = 0$

## Cosa fa

- Integra l'equazione per n = 1, 1.5, 3
- Trova il primo zero $\xi_1$ per interpolazione lineare
- Ricava a e $\rho_C$ imponendo
  massa e raggio del Sole
- Disegna $\theta(\xi)$, $\rho(r)$ in scala logaritmica e m(r)/M in 3 canvas separate

## Organizzazione dei file

Il programma principale fa uso di alcune classi, definite ciascuna in un file .h
e implementate in un file .cpp. In particolare abbiamo definito:

- Vector3: classe che gestisce vettori tridimensionali
- Particle: classe che gestisce particelle massive, eventualmente cariche
- MatPoint: classe ereditata da Particle, che gestisce punti materiali non carichi
- OdeSolver: classe per risolvere numericamente equazioni differenziali.
             Permette di definire fInternal e fExternal con funzioni arbitrarie e
             di aggiungere un numero qualsiasi di punti materiali da passare dentro
             un vector di MatPoint. Si può settare un incremento del passo e scegliere
             l'algoritmo da usare per l'integrazione tra Eulero, Runge-Kutta 2 e Runge-Kutta 4.
             Il metodo fondamentale è Step(), che fa avanzare l'algoritmo del passo
             settato.

## Struttura del main

- Per risolvere l'equazione di Lane-Emden usando la classe MatPoint, abbiamo definito
innanzitutto fInternal e fExternal. La prima restituisce un Vector3 nullo, la seconda invece
sfrutta la forma della derivata seconda data dall'equazione.
- Dal momento che OdeSolver utilizza un vector di MatPoint, mentre l'equazione di Lane-Emden è
scalare, abbiamo deciso di lavorare solo sulla prima componente di un MatPoint, settando le altre
due componenti a zero.
- In zero c'è una singolarità, quindi settiamo il valore iniziale di $\xi$ a 0.01 e facciamo
partire l'integrazione da questo valore.
- Continuiamo a integrare fino a quando raggiungiamo uno zero, momento in cui arrestiamo l'integrazione.
Otteniamo il valore $xi_1$, corrispondente allo zero della soluzione, attraverso un'interpolazione lineare
e lo salviamo.
- Imponendo massa e raggio del Sole, otteniamo i valori di a e di $\rho_C$ e li stampiamo a schermo per
ogni n.
- In una diversa canvas facciamo il grafico di $\rho(r)$ in scala logaritmica.
- In un'altra canvas facciamo il grafico di m(r)/M, dove M rappresenta la massa del Sole.
- In una canvas diversa confrontiamo la soluzione ottenuta numericamente per n=0 con la soluzione
teorica prevista, vale a dire il seno cardinale.
- Nell'ultima canvas viene disegnata la soluzione che si ottiene con n=5. Quello che si osserva è che
in questo caso non ci sono intersezioni della soluzione con l'asse delle x.


## Requisiti

- Compilatore C++ (clang++)
- [ROOT](https://root.cern) per i grafici

## Compilazione ed esecuzione

    make
    ./main

## Risultati

| n   | xi_1   | a [m]       | $\rho_C$ [kg/m³] |
|-----|---------|-------------|------------------|
| 1   | 3.14163 | 2.21445e+08 | 4638.03          |
| 1.5 | 3.6538  | 1.90405e+08 | 8445.63          |
| 3   | 6.89689 | 1.00872e+08 | 76385.5          |

Il valore di riferimento per i modelli solari di $\rho_C è 1.6 \times 10^{5}\ \mathrm{kg/m^{3}}$.
L'indice che lo approssima meglio risulta quindi n=3.

Confrontando la soluzione numerica per n=0 con il seno cardinale, osserviamo che abbiamo ottenuto un'ottima approssimazione della soluzione.

Per n=5 non ci sono intersezioni con l'asse x. Questo non ha fisicamente senso, perché implicherebbe che il Sole ha raggio infinito.
