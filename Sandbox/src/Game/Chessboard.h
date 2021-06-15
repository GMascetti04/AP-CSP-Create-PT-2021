#pragma once
#include "xpch.h"
#include "Engine.h"
struct BoardPosition {
	BoardPosition() {}
	BoardPosition(int _r, int _c) : r(_r), c(_c) {}
	std::tuple<int, int> getPosition() { return std::tuple<int,int>(r,c); }
	int r; int c;
};
enum  PieceColor {
	black = 0, white = 1
	
};





enum  PieceType {
	
	rook = 0, bishop = 1, knight = 2, queen = 3, pawn = 4, king = 5

};



class ChessPiece;


class BoardScript : public  Script {
public:
	~BoardScript() {}


	int pR, pC;

	static Texture* Pieces[2][6];

	enum class State {
		Play = 0, Checkmate = 1, Stalemate = 2, Pawn_Promotion = 3
	};

	State m_currentState;


	void fillBackRows(PieceColor color, int row);


	void fillPawnRow(PieceColor color, int row);

	void ClearBoard();
	ChessPiece* m_board[8][8];
	void promotePawn(ChessPiece* promotion);
	static int s_Instances;
	enum class Mode { //Drag is move, but it's when a piece is held
		//Select: when a player selects a piece to move (the beggining of a turn)
		//Drag: starts when a user selects a piece and ends when the mouse button is released
		//move: starts when the mouse button is released
		Select, Move, Drag
	};

	

	Mode m_mode;
	PieceColor m_currentMove;
	ChessPiece* kingPointers[2];



	const int rows[2] = { 0,7 };

	void Init();



	BoardPosition selectedPosition;
	void unhighlightBox();


	void performMove(int r, int c);

	virtual void onCreate() override;

	virtual void onDestroy() override;

	//-----
	std::vector<BoardPosition> m_moveDots;

	bool inCheck(PieceColor player);
	void SelectPiece(int r, int c);



	std::vector<BoardPosition> computeLegalMoves(ChessPiece* piece);


	struct PieceData {
		PieceColor m_color; PieceType m_type;
	};
	std::vector<PieceData> m_captures[2];

	void movePiece(BoardPosition source, BoardPosition destination, bool test = false, ChessPiece** captured = (ChessPiece**)nullptr);



	bool noLegalMoves(PieceColor player);
	

	std::vector<BoardPosition> canCastle(PieceColor player);


	Vec4<unsigned char> BoardColors[3] = { {181,137,102,255},
		{242,218,182,255},{200,50,100,255} };


	void highlightBox(int r, int c);


	

	std::tuple<int, int> getBox(float x, float y);


	

	//--------
	virtual void onEvent(Event& event) override;


	void SnapPiece();
	virtual void onGuiUpdate() override;

	ChessPiece* pieceAt(BoardPosition& pos)
	{
		return m_board[pos.r][pos.c];
	}

};

