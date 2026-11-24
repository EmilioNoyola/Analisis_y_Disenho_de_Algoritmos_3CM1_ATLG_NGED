<h1 align="center">Análisis y Diseño de Algoritmos</h1>

<p align="center"><strong>Escuela Superior de Cómputo · Instituto Politécnico Nacional</strong><br>Ingeniería en Sistemas Computacionales · Tercer semestre</p>

<p align="center">
  <a href="https://www.escom.ipn.mx/htmls/oferta/mapaCurrISC2020.php"><img src="https://img.shields.io/badge/ISC-Plan_2020-6D1A42?style=flat-square" alt="ISC · Plan 2020"></a>
  <img src="https://img.shields.io/badge/Semestre-III-334155?style=flat-square" alt="Semestre III">
  <img src="https://img.shields.io/badge/Grupo-3CM1-1E5A7A?style=flat-square" alt="Grupo 3CM1">
</p>

<p align="center"><a href="#programa-de-estudios">Programa de estudios</a> &nbsp;·&nbsp; <a href="#contenido-del-repositorio">Contenido</a> &nbsp;·&nbsp; <a href="#uso-del-material">Uso del material</a> &nbsp;·&nbsp; <a href="#referencias">Referencias</a></p>

---

Implementaciones en C y JavaScript, mediciones experimentales y reportes sobre eficiencia algorítmica. El recorrido del repositorio va de la medición de tiempos a divide y vencerás, algoritmos ávidos y programación dinámica.

| Acceso directo | Qué encontrarás |
|---|---|
| [Ver las prácticas](03_practicas/) | Código, enunciados y reportes por actividad. |
| [Preparar el segundo parcial](04_guias/g03_repaso_resuelto_segundo_parcial.pdf) | Repaso resuelto para trabajar junto a las guías anteriores. |
| [Preparar el tercer parcial](04_guias/g04_tercer_parcial.pdf) | Programación dinámica, complejidad, reducciones y problemas NP-completos. |
| [Revisar LCS](01_ejercicios/ejercicio_rapido_lcs/) | Enunciado y solución de subsecuencia común más larga. |

## Programa de estudios

Comparar soluciones computacionales mediante su costo y elegir estrategias de diseño adecuadas para cada problema.

Síntesis del [programa oficial de la unidad de aprendizaje](https://www.escom.ipn.mx/docs/oferta/uaISC2020/analisisDisenoAlgoritmos_ISC2020.pdf), perteneciente al **Plan ISC 2020**.

**Carga semanal:** 3 horas de teoría y 1.5 de práctica. **Créditos:** 7.5 TEPIC / 5.9 SATCA.

| Unidad | Eje de estudio | Alcance |
|:---:|---|---|
| I | Costo de los algoritmos | Complejidad temporal, crecimiento y análisis asintótico. |
| II | Métodos de diseño | Divide y vencerás, decisiones ávidas y programación dinámica. |
| III | Complejidad de problemas | Problemas de la clase NP y límites de la resolución eficiente. |

**Antecedentes académicos:** Fundamentos de Programación, Algoritmos y Estructuras de Datos, Matemáticas Discretas y Cálculo.

El programa describe el alcance de la asignatura. Las carpetas siguientes muestran el material efectivamente disponible en esta colección.

## Contenido del repositorio

| Carpeta | Contenido |
|---|---|
| [Ejercicios](01_ejercicios/) · `01_ejercicios` | Subarreglos, matrices, inversiones, divide y vencerás y LCS. |
| [Tareas](02_tareas/) · `02_tareas` | Problema del número mínimo de monedas. |
| [Prácticas](03_practicas/) · `03_practicas` | Medición de tiempos, inversiones, planificación de eventos y mochila 0/1 con programación dinámica. |
| [Guías](04_guias/) · `04_guias` | Primer, segundo y tercer parcial; repaso resuelto del segundo parcial. |
| [Material de clase](05_material_clase/) · `05_material_clase` | Heaps, colas de prioridad, Huffman, Dijkstra y programación dinámica. |
| [Exámenes de referencia](06_examenes/) · `06_examenes` | Dos exámenes pasados resueltos. |
| [Apuntes](07_apuntes/) · `07_apuntes` | Cuaderno de clase. |

Los números de actividad conservan la identificación de las entregas; las guías se distinguen por el prefijo `g` y las prácticas por `p` o `act`, según la materia.

## Uso del material

### Ejecutar una implementación

Los programas C requieren un compilador como GCC; los scripts JavaScript se ejecutan con Node.js. Las visualizaciones HTML se abren en un navegador. Trabaja desde la raíz del repositorio y conserva junto al código los archivos de datos de cada práctica.

<details>
<summary><strong>Ejemplo en PowerShell: planificación de eventos</strong></summary>

```powershell
New-Item -ItemType Directory -Force build | Out-Null
gcc .\03_practicas\p02_03_algoritmos_avidos_i\planificacion.c -o .\build\planificacion.exe
.\build\planificacion.exe
```

El código de [planificación ávida](03_practicas/p02_03_algoritmos_avidos_i/planificacion.c) y el de [planificación ponderada](03_practicas/p02_04_programacion_dinamica_i/planificacion_ponderada.c) pertenecen a actividades distintas. Consulta sus reportes para comparar el criterio de selección y el problema que resuelve cada estrategia.

</details>

### Leer los resultados

1. Revisa el enunciado y la estrategia antes de ejecutar el programa.
2. Contrasta el código con las mediciones, tablas o visualizaciones de la misma práctica.
3. Al comparar tiempos, registra equipo, compilador, tamaño de entrada y repeticiones; un tiempo aislado no describe la complejidad asintótica.

<details>
<summary><strong>Clonar el repositorio</strong></summary>

```powershell
git clone https://github.com/EmilioNoyola/Analisis_y_Disenho_de_Algoritmos_3CM1_ATLG_NGED.git analisis_algoritmos
cd analisis_algoritmos
```

</details>

## Referencias

- [Programa oficial de Análisis y Diseño de Algoritmos — PDF](https://www.escom.ipn.mx/docs/oferta/uaISC2020/analisisDisenoAlgoritmos_ISC2020.pdf): unidades, carga académica, evaluación y bibliografía institucional.
- [Catálogo de unidades de aprendizaje de ISC 2020](https://www.escom.ipn.mx/htmls/oferta/mapaCurrISC2020.php): programas oficiales publicados por ESCOM.
- [Mapa curricular de ISC 2020 — PDF](https://www.escom.ipn.mx/docs/oferta/mapaCurricularISC2020.pdf): ubicación de la materia en la carrera.

---

**Repositorios relacionados:** [Paradigmas de Programación](https://github.com/EmilioNoyola/Paradigmas_de_Programacion_3CM3_NGED) · [Bases de Datos](https://github.com/EmilioNoyola/Bases_de_Datos_3CM3_ATLG_HX_NGED_FSJM).

Colección académica de [Emilio Noyola](https://github.com/EmilioNoyola). La autoría de cada entrega se indica en su portada o en el código correspondiente.
