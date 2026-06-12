# Tron - Battle Tanks

A 2D recreation of the classic arcade game, built using a custom C++ game engine. 
Developed for Programming 4 at DAE.

**Luca De Meyer**
https://github.com/LucaDeMeyer/programming4_Engine


## 🎮 Game Modes
* **Single Player:** Survive endless waves of enemy Tanks and Recognizers.
* **Co-Op:** Team up with a friend locally to take down the AI.
* **PvP:** Go head-to-head against another player.


## 🕹️ Controls
**Player 1 (Keyboard)**
* **Move:** W A S D
* **Aim:** Arrow Keys
* **Fire:** Spacebar

**Player 2 (Controller)**
* **Move:** D-Pad
* **Aim:** Right Thumbstick
* **Fire:** Right Bumper (R1/RB)


  
## ⚙️ Applied Programming Patterns
* **Components:** Used heavily throughout the engine to compose `GameObjects`. Behaviors and data are separated into modular, reusable parts like `TransformComponent`, `BoxColliderComponent`, and `LivesComponent` rather than using deep, rigid inheritance hierarchies.
* **State Pattern:** Used for managing Game States (Menu, Gameplay, High Scores) and Enemy AI behaviors (Patrolling, Chasing, Attacking).
* **Command Pattern:** Used in the `InputManager` to decouple physical button presses from in-game actions, allowing for easy rebinding and supporting multiple controller inputs seamlessly.
* **Observer Pattern:** Used for the UI to listen to changes in Player Lives and Score without the gameplay code needing to know about the UI elements.
* **Service Locator:** Used to provide global, safe access to the Audio system (`AudioService`) without relying on a rigid Singleton.
* **Flyweight:** Applied to the Level generation. Instead of creating hundreds of individual `GameObject` entities for every wall and path, the grid data is shared and rendered efficiently through a single `TileMapComponent` to save memory.
* **Type Object:** Used for enemy spawning (`TronFactory`). Instead of creating hardcoded C++ classes for every enemy type, a generic enemy GameObject is configured via data (e.g., `AIType::Tank` or `AIType::Recogniser`) to define its specific stats and behaviors.
* **Event Queue:** Utilized for both the multithreaded Audio System and Collision Handling. Audio commands (play/pause/mute) are pushed to a thread-safe worker queue so sound processing never stutters the main loop. Similarly, collision events are queued and dispatched asynchronously, cleanly decoupling the raw physics math from gameplay reactions (like taking damage or spawning explosions).
* **Object Pool:** Implemented in the `BulletManager` (and `ParticleManager`). Projectiles and explosions are pre-allocated and recycled rather than constantly allocating and deallocating memory, preventing performance drops during heavy combat.
