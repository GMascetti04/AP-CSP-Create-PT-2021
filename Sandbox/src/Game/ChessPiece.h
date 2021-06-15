#pragma once
#include "Engine.h"
#include "Chessboard.h"


class ChessPiece
{
public:
	virtual std::vector<BoardPosition> computePossibleMoves() = 0;
	
	virtual ~ChessPiece() {
		m_entity.getScene()->destroyEntity(m_entity);
	}

	PieceColor getColor() { return m_color; }
	virtual PieceType getPieceType() = 0;

	std::tuple<int, int> getPosition()
	{
		
		for (int r = 0; r < 8; r++)
			for (int c = 0; c < 8; c++)
				if (m_boardScript->m_board[r][c] == this)
					return std::tuple<int, int>(r,c);
		return std::tuple<int, int>(-1, -1);
	}
	Entity getEntity() { return m_entity; }

private:

protected:
	friend class ChessBoard;
	Entity m_entity;
	const PieceColor m_color;
	BoardScript* m_boardScript;

	ChessPiece(PieceColor color, Entity chessboard, int r, int c) : m_color(color) {

		m_entity = chessboard.getScene()->createEntity();
		m_entity.addComponent<TransformComponent>();
		m_entity.addComponent<Sprite2DComponent>().visible = true;
		Sprite2DComponent& sprite = m_entity.getComponent<Sprite2DComponent>();
		sprite.layer = "Pieces";
		sprite.color = { 1.0f,1.0f,1.0f,1.0f };
		TransformComponent& ts = m_entity.getComponent<TransformComponent>();

		TransformComponent& BoardTS = chessboard.getComponent<TransformComponent>();
		ts.x_scale = BoardTS.x_scale / (8.0f);
		ts.y_scale = BoardTS.y_scale / (8.0f);			
		ts.x = (BoardTS.x - BoardTS.x_scale / 2) + ts.x_scale / 2 + ts.x_scale * c;
		ts.y = (BoardTS.y + BoardTS.y_scale / 2) - ts.y_scale / 2 - ts.y_scale * r;

		m_boardScript = (BoardScript*)chessboard.getComponent<ScriptComponent>().m_Script;

	}


};

//-----------------------Specific Pieces

class Pawn : public ChessPiece {
public:
	Pawn(PieceColor color, Entity chessboard, int r, int c) : ChessPiece(color, chessboard, r, c) {
		Sprite2DComponent& sprite = m_entity.getComponent<Sprite2DComponent>();
		sprite.texture = BoardScript::Pieces[(int)color][(int)PieceType::pawn];
		m_moves = 0;
	}

	virtual std::vector<BoardPosition> computePossibleMoves() override;
	virtual PieceType getPieceType() override { return PieceType::pawn; }
private:
	int m_moves;
};


class Knight : public ChessPiece {
public:
	Knight(PieceColor color, Entity chessboard,int r, int c) : ChessPiece(color, chessboard, r, c) {
		Sprite2DComponent& sprite = m_entity.getComponent<Sprite2DComponent>();
		sprite.texture = BoardScript::Pieces[(int)color][(int)PieceType::knight];
	}

	virtual std::vector<BoardPosition> computePossibleMoves() override;
	virtual PieceType getPieceType() override { return PieceType::knight; }
private:
};

class Rook : public ChessPiece {
public:
	Rook(PieceColor color, Entity chessboard, int r, int c) : ChessPiece(color, chessboard, r, c) {
		Sprite2DComponent& sprite = m_entity.getComponent<Sprite2DComponent>();
		sprite.texture = BoardScript::Pieces[(int)color][(int)PieceType::rook];	
	}

	virtual std::vector<BoardPosition> computePossibleMoves() override;
	virtual PieceType getPieceType() override { return PieceType::rook; }
private:
	friend class Chessboard;
	friend class BoardScript;
	int m_timesMoved = 0;
};

class Queen : public ChessPiece {
public:
	Queen(PieceColor color, Entity chessboard, int r, int c) : ChessPiece(color, chessboard, r, c) {
		Sprite2DComponent& sprite = m_entity.getComponent<Sprite2DComponent>();
		sprite.texture = BoardScript::Pieces[(int)color][(int)PieceType::queen];
	}

	virtual std::vector<BoardPosition> computePossibleMoves() override;
	virtual PieceType getPieceType() override { return PieceType::queen; }
private:
};

class Bishop : public ChessPiece {
public:
	Bishop(PieceColor color, Entity chessboard, int r, int c) : ChessPiece(color, chessboard, r, c) {
		Sprite2DComponent& sprite = m_entity.getComponent<Sprite2DComponent>();
		sprite.texture = BoardScript::Pieces[(int)color][(int)PieceType::bishop];
	}

	virtual std::vector<BoardPosition> computePossibleMoves() override;
	virtual PieceType getPieceType() override { return PieceType::bishop; }
private:
};


class King : public ChessPiece {
public:
	King(PieceColor color, Entity chessboard, int r, int c) : ChessPiece(color, chessboard, r, c) {
		Sprite2DComponent& sprite = m_entity.getComponent<Sprite2DComponent>();
		sprite.texture = BoardScript::Pieces[(int)color][(int)PieceType::king];
	}
	

	virtual std::vector<BoardPosition> computePossibleMoves() override;
	virtual PieceType getPieceType() override { return PieceType::king; }

private:
	friend class Chessboard;
	friend class BoardScript;
	int m_timesMoved = 0;

};


