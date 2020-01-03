enum Color {
	WHITE,
	RED,
	BLUE,
	YELLOW,
	GREEN
};

enum FigureState {
	IN_HOUSE,
	IN_GAME,
	IN_FINISH
};

enum GameState {
	WAITING,
	PLAYING,
	FINISHED
};

struct Figurine {
	FigureState figurineState;
	int position;
};

struct Player {
	int playerId;
	Figurine figurines[4];
};

struct Game {
	GameState gameState;
	Player players[4];
	int turnId;
};
