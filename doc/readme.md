# somigame
- pixel-style lo-fi 2.5d top-down keyboard controlled game
- this is now being developed
> and unfortunately cannot be played
## game and programming design decisions
- there are some things that may seem weird
> but i think about everything in this game
> almost everything written in code or expressed in files
> > everything was controlled and processed by my mind
### there is no randomness
- every move made by the hero predictably changes the world
- i just hate randomness
> like in terraria, where i am gambling rather than testing with skills
- this game is more similar to the chess game
> but there are even more variations
### 2.5d top-down
- the view type of my game lets me show more without overcomplication
> the plain 2d seems quite limiting to me
> 3d is not about pixel art
- all objects are 2d, but they are located in layers of other 2d planes
### turn based
- initially it was supposed to have physics and runtime gameplay
> physics turned out to be pretty bad with my integer math
> > this is just a nightmare of integer division loss
> > so i could fix this only using really big integer numbers
- processing the game with events is much easier and more performant
### keyboard driven
### basic frameworks and apis
- i did not and do not want to develop this game using a game engine
> this project has very specific things
> > that i think are better to be implemented in low-level
- other than my specific needs and game engine's limitations
> i just enjoy controlling things and using what i need
### integer only
- i decided to build the game with integer numbers as much as i can
> of cource there are performance improvements because of that
> > but this is rather my challenge
> there are important advantages to the style of my game like
> > pixel-art precision: every coordinate can correspond to a pixel
> > easily controlled aspect ratio and viewport
> i just do not have any need for anti-aliasing or smooth rendering
- i know how to work with floats
> i know their limitations and how precision works
### naming convention
- you may notice that most of the names in this project
> have abbreviations
> are screwed in some way
> and some logical groups share the same name length
- just, don't tell me my names are weird
> this is my project, my conventions
> and i think this is the best way to name things
### version convention
- 0xa1a2a3 - this is my personal way to write versions
> 0x being the hexadecimal full-version prefix
> 'a' being a sub-version delimiter, meaning "at"
> > this is also 10 in hex
> 1 for the major version
> 2 for the minor version
> 3 for the patch version
- the main reason why i like this convention
> it makes more sense to me
> this is a fixed 8-character-long value
> written in hex, so it can be parsed as a number
- don't be so annoyed with it if you are
> this is just how i wanna write it
## TODO
- there are many features and ideas i wanna implement
> and i can at least write them down here
> this will also explain some of my further dev process
### graphics
- entity directions
> there should be 4 directions that any entity can look at
> > camera's point of view is technically gonna rotate the whole world
> camera's dir relative to an entity's dir should affect it's view
- animations
> every action can trigger an animation
> > nothing can be done until animation is played/skipped
> > animation should be skippable
- text alignment relative to different parts of a text label
> i want each glyph to align like all my quads do with pivots and rposes
### world
- other than tiles and the main hero, we should have other objects
> monsters
> critters
> grass
> trees
- tiles should have types defined by an attached enum
> the enum is gonna be the key to a type lookup table
- each tile should be diying over time - player turns
> different tile types can have different lifetime
> the bigger distance between the player and a tile
> > the faster it dies
> > the tile/tiles under the player cannot die
### gameplay
- turns should be the measure of time
- most of the player's actions are gonna be turns
> after player's action - almost all game entities should react
> > all reactions are gonna be dependent on something
- life-valued currency
> there has to be some measure of health and power at the same time
> each tile can bring some of this currency
- world's dependency on the hero
> the hero state has to change the state of the entire world
- there should be a magic system
> different types of magic
> spells created by combinations of more low-level spells
- we need building
> player is gonna be able to build tiles and create objects on them
> what the hero can build should depend on their powers
- there should be a skill tree
> few branches that should give different useful abilities
### audio
- i wanna have music and sounds
> separate namespace with its own components
> a minimalistic library for 2d sound
> > there are advices to use portaudio for such cases
### interface
- there should be some panels and windows to open
> status window
- i can also have bars always-on-top
> at least one to display the keyboard information
- i also wanna have some keybind helper window
> which is gonna display all keybinds available at any moment
> > similarly to the "vieb" web browser
- keyboard controls are nice
> but the system is not that clean right now
> i wanna improve the keyboard organization
> > using templates, classes and signals
- i wanna have settings
> there should be a module for changing properties of the game
> > aspect ratio
> > sound
> > maybe even color palette
- graphical user interfaces should be separate
> like divided by groups... main hud, menu, settings, loading, etc...
### organization
- man-type-pattern - i've just invented this one... y23m07d07-h19m49
> this is a pattern of managers who manage their particular values
> the idea is having a tree of classes that handle their own types
> possible use-cases that i need that for
> > keyman - keyboard manager
> > > hierarchies of keymodes and their keybindings
> > optman - options manager
> > > hierarchies of option menus and values
> > guiman - graphical user interfaces manager
> > > gui, hierarchies of menus and widgets
> possible implementation
> > make an abstract class of the type we need that for
> > > optman for example
> > > it needs an array of pointers to other sub-mans
> > > it also needs arrays of the actual values it is taking care of
> > > getter and setter methods
> > > it can also have a link to it's preceeding pointer
> > there should always be a top-level entry that handles the hierarchy
## this file...
- i do not think i am good at writing readme.md
> but i feel i should add it anyways
> maybe i will make it much better in the future
- this is a good way to explain some of my ideas and decisions
- dont't worry if it looks dirty now
> i just wanna have some readme with my actual thoughts
- i should add the license file as well
> just struggling to choose
