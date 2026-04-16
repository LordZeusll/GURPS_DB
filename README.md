# GURPS Space Star System Generator
A procedural star system generator written in C++, based on the world generation rules from the GURPS Space tabletop RPG sourcebook.
## What it does
The generator implements the GURPS Space stellar generation ruleset, including:

* Randomized number of stars per system (1–3) using weighted dice rolls
* Stellar mass generation via lookup tables
* System age calculation
* Stellar characteristics derivation - spectral type, temperature, luminosity, and evolutionary stage (main sequence, subgiant, giant, white dwarf)
* PCG (Permuted Congruential Generator) random number library for high-quality randomness
* Basic database file management for persistent storage

## Status
Work in progress. Core stellar generation logic is implemented. Output and full system assembly are incomplete.
Planned features

* Complete generation pipeline using all GURPS Space rulebook steps
* Planetary system generation per star
* Custom/manually defined star systems and planets
* Interstellable map with randomized distances and connections between systems
* Full database persistence for generated galaxies
