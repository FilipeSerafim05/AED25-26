# Custom Dijkstra Implementation

**Authors:** Filipe Serafim, Lena Wang  
**Course:** AED 2025/2026

## Overview
This project implements a **customized Dijkstra algorithm** to find the shortest path between cities in a transport network. The algorithm can optimize for either **duration** or **cost** while respecting user-defined constraints.

Key features:
- Finds shortest paths based on either **duration** or **cost**.
- Supports multiple **restrictions**:
  - Avoid certain transport types.
  - Limit duration or cost for individual connections.
  - Limit total duration or total cost for the entire trip.
- Handles multiple clients and writes results to an output file.

---

## Input Files

1. **Map file (`.map`)**  
   The **first line** contains: <br>
    numberOfCities numberOfConnections <br>
   Each subsequent line describes a connection:
    origin destination transport duration cost firstDeparture lastDeparture departurePeriodicity


3. **Clients file (`.cli`)**  
   The **first line** contains: <br>
    numberOfClients <br>
   Each subsequent line describes a client:
    clientID startingCity endingCity departureTime filter numRestrictions [restrictions]
   - `filter` can be `cost` or `duration`.  
   - `0 <= numRestrictions <= 2`.  
   - Restrictions include `A1`, `A2`, `A3`, `B1`, `B2`.  

---

## Output File
- A `.sol` file is generated for each client input file.  
- Each line contains either:
- `<clientID> -1` if **no** valid path exists.  
- `<clientID> startingCity <transport city ...> endingCity duration cost` for valid paths.

---

## How to Build and Run

Requires a C compiler (tested with `gcc` on Ubuntu).

```bash
make

./tourists <file.map> <file.cli>
```
The results will be written to a `.sol` file with the same name as the `.cli` input file.

## Example

### Map file (`example.map`)

3 3 <br>
1 2 bus 60 10 480 1020 60 <br>
2 3 train 120 20 540 1140 120 <br>
1 3 plane 90 50 600 1200 180 <br>

- The first line `3 3` means **3 cities** and **3 connections**.
- Each subsequent line describes a connection:
  - `origin destination transport duration cost firstDeparture lastDeparture departurePeriodicity`

---

### Clients file (`example.cli`)

2 <br>
1 1 3 500 duration 1 A2 100 <br>
2 2 1 600 cost 0 <br>

- The first line `2` means there are **2 clients**.
- Each subsequent line describes a client request:
  - `clientID startingCity endingCity departureTime filter numRestrictions [restrictions]`
  - Client 1: prefers shortest duration, with restriction `A2` (max connection duration 100).
  - Client 2: prefers lowest cost, no restrictions.

---

### Output file (`example.sol`)

1 1 plane 3 90 50 <br>
2 2 train 3 bus 1 80 30 <br>

- Each line shows:
  - `clientID startingCity <transport city ...> endingCity duration cost`
- If a client request cannot be satisfied due to restrictions, the line would be:
  - <clientID> -1
  
