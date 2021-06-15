#include "xpch.h"
#include "Engine.h"
#include "Chessboard.h"
#include "ChessPiece.h"

void BoardScript::fillBackRows(PieceColor color, int row)
{

	m_board[row][0] = new Rook(color, this->getEntity(), row, 0);
	m_board[row][1] = new Knight(color, this->getEntity(), row, 1);
	m_board[row][2] = new Bishop(color, this->getEntity(), row, 2);
	m_board[row][3] = new Queen(color, this->getEntity(), row, 3);
	m_board[row][4] = new King(color, this->getEntity(), row, 4);
	m_board[row][5] = new Bishop(color, this->getEntity(), row, 5);
	m_board[row][6] = new Knight(color, this->getEntity(), row, 6);
	m_board[row][7] = new Rook(color, this->getEntity(), row, 7);
}

void BoardScript::fillPawnRow(PieceColor color, int row)
{
	for (int c = 0; c < 8; c++)
		m_board[row][c] = new Pawn(color, this->getEntity(), row, c);
}

void BoardScript::ClearBoard()
{
	for (int r = 0; r < 8; r++)
		for (int c = 0; c < 8; c++)
			if (m_board[r][c])
			{
				delete m_board[r][c];
				m_board[r][c] = nullptr;
			}

	m_moveDots.clear();
	for (int i = 0; i < 2; i++)
	{
		m_captures[i].clear();
	}
}

void BoardScript::promotePawn(ChessPiece* promotion)
{
	for (int c = 0; c < 8; c++)
	{
		if (m_board[0][c] && (m_board[0][c]->getPieceType() == PieceType::pawn))
		{
			delete m_board[0][c];
			m_board[0][c] = promotion;
			return;
		}
		if (m_board[7][c] && (m_board[7][c]->getPieceType() == PieceType::pawn))
		{
			delete m_board[7][c];
			m_board[7][c] = promotion;
			return;
		}
	}
}

bool BoardScript::inCheck(PieceColor player)
{
	auto [kingRow, kingColumn] = kingPointers[player]->getPosition();

	for (int row = 0; row < 8; row++)
	{
		for (int column = 0; column < 8; column++)
		{
			if ((m_board[row][column] != nullptr) && (m_board[row][column]->getColor() != player))
			{
				std::vector<BoardPosition> oponentMoves = m_board[row][column]->computePossibleMoves();
				for (int i = 0; i < oponentMoves.size(); i++)
				{
					if ((oponentMoves[i].r == kingRow) && (oponentMoves[i].c == kingColumn))
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

void BoardScript::SelectPiece(int r, int c)
{
	m_moveDots.clear();
	unhighlightBox();
	m_moveDots = computeLegalMoves(m_board[r][c]);
	selectedPosition = { r,c };

	highlightBox(r, c);
}


//computes the moves that the piece can make
std::vector<BoardPosition> BoardScript::computeLegalMoves(ChessPiece* piece)
{
	std::vector<BoardPosition> legalMoves = piece->computePossibleMoves();
	auto [rowOriginal, columnOriginal] = piece->getPosition();
	
	ChessPiece* temp;

	for (std::vector<BoardPosition>::iterator it = legalMoves.begin(); it != legalMoves.end();) 
	{
		auto [r, c] = it->getPosition();
	
		temp = m_board[r][c];
		m_board[r][c] = m_board[rowOriginal][columnOriginal];
		m_board[rowOriginal][columnOriginal] = nullptr;

		bool checked = this->inCheck(piece->getColor());
		
		m_board[rowOriginal][columnOriginal] = m_board[r][c];
		m_board[r][c] = temp;

		if (checked)
		{
			it = legalMoves.erase(it);
			
			if (it != legalMoves.begin())
				it = std::prev(it);
		}
		else
		{
			it++;
		}
	}

	

	if (piece->getPieceType() == PieceType::king) {
		
		std::vector<BoardPosition> possibleCastles = canCastle(piece->getColor());
		legalMoves.insert(legalMoves.end(), possibleCastles.begin(), possibleCastles.end());
	}

	return legalMoves;
}

void BoardScript::movePiece(BoardPosition source, BoardPosition destination, bool test, ChessPiece** captured)
{

	auto [rSource, cSource] = source.getPosition();
	TransformComponent& ts = m_board[rSource][cSource]->getEntity().getComponent<TransformComponent>();
	TransformComponent& BoardTS = getComponent<TransformComponent>();
	//r and c are the destination
	auto [r, c] = destination.getPosition();
	

	ts.x = (BoardTS.x - BoardTS.x_scale / 2.0f) + ts.x_scale / 2 + ts.x_scale * c;
	ts.y = (BoardTS.y + BoardTS.y_scale / 2.0f) - ts.y_scale / 2 - ts.y_scale * r;
	unhighlightBox();


	if (m_board[r][c]) //capture a piece
	{
		if (!test)
			m_captures[this->m_currentMove].push_back(PieceData{ this->m_board[r][c]->getColor(),this->m_board[r][c]->getPieceType() }); //color then type

		if (captured)
			*captured = m_board[r][c];

		if (!test)
			delete m_board[r][c];
	}
	this->m_board[r][c] = this->m_board[rSource][cSource];
	this->m_board[rSource][cSource] = nullptr;

	if (test == false)
	{
		if (this->m_board[r][c]->getPieceType() == PieceType::king)
		{
			((King*)(m_board[r][c]))->m_timesMoved++;
		}
		if (this->m_board[r][c]->getPieceType() == PieceType::rook)
		{
			((Rook*)(m_board[r][c]))->m_timesMoved++;
		}
	}

}

bool BoardScript::noLegalMoves(PieceColor player)
{
	for (int r = 0; r < 8; r++) {
		for (int c = 0; c < 8; c++) {
			if (m_board[r][c] && (m_board[r][c]->getColor() == player)) {
				std::vector<BoardPosition> possibleMoves = computeLegalMoves(m_board[r][c]);
				if (possibleMoves.size() > 0)
					return false;
			}
		}
	}
	return true;
}



std::vector<BoardPosition> BoardScript::canCastle(PieceColor player)
{
	std::vector<BoardPosition> possibleCastles;

	int rows[2];
	rows[PieceColor::black] = 0;
	rows[PieceColor::white] = 7;


	if (inCheck(player))
		return possibleCastles;
	
	if (((King*)(kingPointers[player]))->m_timesMoved != 0)
		return possibleCastles;

	//check left
	//is the piece there a rook of the same color as player
	if(m_board[rows[player]][0])
	if (m_board[rows[player]][0]->getPieceType() == PieceType::rook && m_board[rows[player]][0]->getColor() == player)
	{
		//did it move?
		if (((Rook*)m_board[rows[player]][0])->m_timesMoved == 0)
		{
			//are the spaces between the king and the rook empty
			for (int i = 3; i > 0; i--)
			{
				if (m_board[rows[player]][i] != nullptr) //there is a piece in between
					goto leftCastleFailed;
				movePiece({ rows[player] ,4 }, { rows[player] , i }, true);
				bool stopped = inCheck(player);
				movePiece({ rows[player] , i }, { rows[player] ,4 }, true);
				if (stopped)
					goto leftCastleFailed;
			}
			//check the end position
			ChessPiece* temp;
			movePiece({ rows[player] ,4 }, { rows[player] , 0 }, true, &temp);
			bool stopped = inCheck(player);
			movePiece({ rows[player] , 0 }, { rows[player] ,4 }, true);
			m_board[rows[player]][0] = temp;
			if (stopped)
				goto leftCastleFailed;
			//made it - the king can castle here
			possibleCastles.push_back({ rows[player], 2 });
		}
	}

leftCastleFailed:
	//check right
	//is the piece there a rook of the same color as player
	if(m_board[rows[player]][7])
	if (m_board[rows[player]][7]->getPieceType() == PieceType::rook && m_board[rows[player]][7]->getColor() == player)
	{
		//did it move?
		if (((Rook*)m_board[rows[player]][7])->m_timesMoved == 0)
		{
			//are the spaces between the king and the rook empty
			for (int i = 5; i < 7; i++)
			{
				if (m_board[rows[player]][i] != nullptr) //there is a piece in between
					goto rightCastleFailed;
				movePiece({ rows[player] ,4 }, { rows[player] , i }, true);
				bool stopped = inCheck(player);
				movePiece({ rows[player] , i }, { rows[player] ,4 }, true);
				if (stopped)
					goto rightCastleFailed;
			}
			//check the end position
			ChessPiece* temp;
			movePiece({ rows[player] ,4 }, { rows[player] , 0 }, true, &temp);
			bool stopped = inCheck(player);

			movePiece({ rows[player] , 0 }, { rows[player] ,4 }, true);
			m_board[rows[player]][0] = temp;
			if (stopped)
				goto rightCastleFailed;
			//made it - the king can castle here
			possibleCastles.push_back({ rows[player], 6 });
		}
	}

rightCastleFailed:

	return possibleCastles;
}






void BoardScript::performMove(int r, int c) {
	//castle
	if ((this->pieceAt(selectedPosition)->getPieceType() == PieceType::king) && abs(c - selectedPosition.c) > 1)
	{
		//determine the side
		if (c < 4) {  //to the left
			movePiece({ rows[m_currentMove],4 }, { rows[m_currentMove],2 }); //move king
			movePiece({ rows[m_currentMove],0 }, { rows[m_currentMove],3 });
		}
		else {    //to the right
			movePiece({ rows[m_currentMove],4 }, { rows[m_currentMove],6 }); //move king
			movePiece({ rows[m_currentMove],7 }, { rows[m_currentMove],5 }); //move king
		}
	}
	else  //non castle-move
	{
		movePiece(selectedPosition, { r,c });
	}
	if ((r == 0 || r == 7) && m_board[r][c]->getPieceType() == PieceType::pawn)
	{
		m_currentState = State::Pawn_Promotion;
		pR = r;
		pC = c;
	}

	m_moveDots.clear();


	if (noLegalMoves(PieceColor(!m_currentMove)))
	{
		if (inCheck(PieceColor(!m_currentMove)))
			m_currentState = State::Checkmate;
		else
			m_currentState = State::Stalemate;
	}

	m_currentMove = PieceColor(!m_currentMove);
	m_mode = Mode::Select;
}




    void BoardScript::onEvent(Event& event) {
    	if (GuiLayer::handledEvent(event) || (m_currentState != State::Play))
    		return;

    	auto [xPos, yPos] = Script::getMousePosition();
    	auto [r, c] = getBox(xPos, yPos);

    	if (Event::isEvent<MouseMoveEvent>(event) && (m_mode == Mode::Drag)) 
			return (pieceAt(selectedPosition)->getEntity().getComponent<TransformComponent>().SetPosition(xPos, yPos));

    	bool mouse_pressed = Event::checkProperty(event, &MouseButtonPressEvent::getButton, codes::MouseCode::left_button);
    	bool mouse_released = Event::checkProperty(event, &MouseButtonReleaseEvent::getButton, codes::MouseCode::left_button);

    	if (mouse_pressed && r > -1 && r < 8 && c > -1 && c < 8 && (m_board[r][c]) && (m_board[r][c]->getColor() == m_currentMove)) { 
			m_mode = Mode::Drag;
			return SelectPiece(r, c);
    	}
    	if ((mouse_released && m_mode == Mode::Drag) || (mouse_pressed)) {
    		for (int i = 0; i < m_moveDots.size(); i++) 
    			if ((m_moveDots[i].c == c) && (m_moveDots[i].r == r)) 
					return performMove(r, c);

    		if((mouse_released && m_mode == Mode::Drag))
    			SnapPiece(); 
    		m_mode = Mode::Move;
    	}
    } 

void BoardScript::SnapPiece()
{
	TransformComponent& ts = this->pieceAt(selectedPosition)->getEntity().getComponent<TransformComponent>();
	TransformComponent& BoardTS = getComponent<TransformComponent>();
	ts.x_scale = BoardTS.x_scale / 8.0f;
	ts.y_scale = BoardTS.y_scale / 8.0f;
	ts.rot = 0.0f;
	ts.x = (BoardTS.x - BoardTS.x_scale / 2.0f) + ts.x_scale / 2 + ts.x_scale * selectedPosition.c;
	ts.y = (BoardTS.y + BoardTS.y_scale / 2.0f) - ts.y_scale / 2 - ts.y_scale * selectedPosition.r;
}

void BoardScript::onGuiUpdate() {
	
	ImGuiWindowFlags win_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

	ImGui::SetNextWindowSize({ 0,0});
	ImGui::SetNextWindowPos({0,0});

	ImGui::Begin(" ", (bool*)0, win_flags);
	
	if (m_currentMove == PieceColor::white)
		ImGui::Text("Current Move: White\n");
	else
		ImGui::Text("Current Move: Black\n");


	if (ImGui::Button("Restart")) {
		ClearBoard();
		Init();
	}

	ImGui::End();
	

	//draw the dots
	if (m_moveDots.size() > 0)
	{
		TransformComponent& ts = Script::getComponent<TransformComponent>();
		float radius = (ts.y_scale / 8.0f) / (float)(Script::getEntity().getScene()->getCameraController()->getCamHeight()) * application::getWindow()->getHeight() / 4.0f;

		for (int i = 0; i < m_moveDots.size(); i++)
		{
			auto [r, c] = m_moveDots[i].getPosition();

			float x = ts.x - (ts.x_scale / 2.0f) + c * ts.x_scale / 8.0f + (ts.x_scale / 16.0f);
			float y = ts.y + (ts.y_scale / 2.0f) - r * (ts.y_scale / 8.0f) - +(ts.y_scale / 16.0f);

			auto [xPos, yPos] = Script::worldToScreenSpace(x, y);

			//this function is ABGR 
			ImGui::GetBackgroundDrawList()->AddCircleFilled({ (float)xPos, (float)yPos }, radius, ((int)(0b11110000'11000000'11000000'11000000)));
		}
	}


	static const char* winners[2] = {"White Won!!","Black won!!"};

	switch (m_currentState)
	{
	case BoardScript::State::Checkmate:
		ImGui::SetNextWindowPos({ 200,200 });
		ImGui::SetNextWindowSize({200,100});
		ImGui::Begin("Game Over - Checkmate", (bool*)0, win_flags);
		ImGui::Text("Game Over - Checkmate");

		ImGui::Text(winners[(m_currentMove == PieceColor::white)]);

		if (ImGui::Button("Play Again")) {
			ClearBoard();
			Init();
		}
		ImGui::End();
		break;

	case BoardScript::State::Stalemate:
		ImGui::SetNextWindowPos({ 200,200 });
		ImGui::SetNextWindowSize({ 200,100 });
		ImGui::Begin("Game Over - Stalemate", (bool*)0, win_flags);
		ImGui::Text("Game Over - Stalemate");
		if (ImGui::Button("Play Again"))
		{
			ClearBoard();
			Init();
		}
		ImGui::End();
		break;


	case BoardScript::State::Pawn_Promotion:
		ImGui::Begin("Pawn Promotion!", (bool*)0, win_flags);
		ImGui::Text("Promote Pawn!");
		for (int i = 0; i < 4; i++)
		{
			if (ImGui::ImageButton((void*)(intptr_t)(BoardScript::Pieces[PieceColor(!m_currentMove)][i]->getId()), ImVec2(100, 100), { 0,1 }, { 1,0 })) {
				ChessPiece* newPiece;
				switch (i)
				{
				case PieceType::bishop:
					newPiece = new Bishop(PieceColor(!m_currentMove), this->getEntity(), pR, pC);
					break;
				case PieceType::knight:
					newPiece = new Knight(PieceColor(!m_currentMove), this->getEntity(), pR, pC);
					break;
				case PieceType::queen:
					newPiece = new Queen(PieceColor(!m_currentMove), this->getEntity(), pR, pC);
					break;
				default:
					newPiece = new Rook(PieceColor(!m_currentMove), this->getEntity(), pR, pC);
					break;
				}
				promotePawn(newPiece);
				if (noLegalMoves(m_currentMove))
				{
					if (inCheck(m_currentMove))
						m_currentState = BoardScript::State::Checkmate;
					else
						m_currentState = BoardScript::State::Stalemate;
				}
				else
					m_currentState = BoardScript::State::Play;
			}
			if (i < 3)
				ImGui::SameLine();
		}

		ImGui::End();
		break;

	default:
		break;

	}


}

Texture* BoardScript::Pieces[2][6];

int BoardScript::s_Instances = 0;

void BoardScript::Init() 
{
	fillBackRows(PieceColor::black, 0);
	fillPawnRow(PieceColor::black, 1);

	fillBackRows(PieceColor::white, 7);
	fillPawnRow(PieceColor::white, 6);

	kingPointers[PieceColor::white] = m_board[7][4];
	kingPointers[PieceColor::black] = m_board[0][4];

	for (int r = 2; r < 6; r++)
		for (int c = 0; c < 8; c++)
			m_board[r][c] = nullptr;

	Sprite2DComponent& sprite = getComponent<Sprite2DComponent>();
	int spriteWidth = sprite.texture->getWidth();
	int spriteHeight = sprite.texture->getHeight();

	int boxWidth = spriteWidth / 8;
	int boxHeight = spriteWidth / 8;

	for (int r = 0; r < 8; r++)
		for (int c = 0; c < 8; c++)
			sprite.texture->paintColoredQuad(c * boxWidth, spriteHeight - boxHeight - boxHeight * r, boxWidth, boxHeight, BoardScript::BoardColors[(int)((r + c) % 2 == 0)]);
		
	m_currentMove = PieceColor::white;
	m_currentState = State::Play;
}


 void BoardScript::onCreate()  {
	if (s_Instances == 0)
	{
		

		/*
		* white rook - https://imgbin.com/png/mh3UfRNq/chess-piece-rook-white-and-black-in-chess-pawn-png
		* Jhopes. White Rook Chess Piece PNG. IMGBIN.com, 18 Oct. 2018, imgbin.com/png/ 
		*	mh3UfRNq/chess-piece-rook-white-and-black-in-chess-pawn-png. Accessed 12 
		*	 May 2021. 
		*/
		Pieces[PieceColor::white][PieceType::rook] = Texture::ImageTexture("res/textures/white rook.png");
		
		/*
		* white knight - https://imgbin.com/png/G5fMdU9A/chess-piece-knight-bishop-king-png
		* Hugelrar21. White Knight Chess Piece PNG. 15 Feb. 2017. IMGBIN.com, 15 Feb. 
		*	2017, imgbin.com/png/G5fMdU9A/chess-piece-knight-bishop-king-png. Accessed 
		*	 12 May 2021. 
		*/
		Pieces[PieceColor::white][PieceType::knight] = Texture::ImageTexture("res/textures/white knight.png");
		
		
		/*
		* white pawn - https://imgbin.com/png/uWA8hu76/chess-piece-black-amp-white-pawn-white-and-black-in-chess-png
		* Sbong. White Pawn Chess Piece PNG. 29 Apr. 2017. IMGBIN.com, 29 Apr. 2017, 
		*	 imgbin.com/png/uWA8hu76/ 
		*	 chess-piece-black-amp-white-pawn-white-and-black-in-chess-png. Accessed 12 
		*	 May 2021. 
		*/
		Pieces[PieceColor::white][PieceType::pawn] = Texture::ImageTexture("res/textures/white pawn.png");
		
		/*
		* white queen - https://imgbin.com/png/v0vn7sNF/chess-piece-queen-king-png
		* Fede337. White Queen Chess Piece PNG. 9 Oct. 2018. IMGBIN.com, 9 Oct. 2018, 
		*	 imgbin.com/png/v0vn7sNF/chess-piece-queen-king-png. Accessed 12 May 2021. 
		*/
		Pieces[PieceColor::white][PieceType::queen] = Texture::ImageTexture("res/textures/white queen.png");
		
		
		/*
		* white bishop - https://imgbin.com/png/zrXJwHdM/chess960-chess-piece-bishop-white-and-black-in-chess-png
		* AwixAwi. Chess Piece White Bishop. 1 Feb. 2017. IMGBIN, 1 Feb. 2017, imgbin.com/ 
		*	png/zrXJwHdM/chess960-chess-piece-bishop-white-and-black-in-chess-png. 
		*	Accessed 16 May 2021. 
		*/
		Pieces[PieceColor::white][PieceType::bishop] = Texture::ImageTexture("res/textures/white bishop.png");
		
		/*
		* white king - https://imgbin.com/png/gL6bQgEa/chess-piece-king-white-and-black-in-chess-chessboard-png
		* TAYLORJOE. Chess Piece White King. 23 Sept. 2018. IMGBIN, 23 Sept. 2018, 
		*	 imgbin.com/png/gL6bQgEa/ 
		*	 chess-piece-king-white-and-black-in-chess-chessboard-png. Accessed 16 May 
		*	2021. 
		*/
		Pieces[PieceColor::white][PieceType::king] = Texture::ImageTexture("res/textures/white king.png");

		/*
		* black rook - https://imgbin.com/png/zHm5YTTJ/chess-piece-rook-white-and-black-in-chess-png
		* Perrywayne001. Chess Piece Black Rook. 18 Dec. 2018. IMGBIN, 18 Dec. 2018, 
		*	imgbin.com/png/zHm5YTTJ/chess-piece-rook-white-and-black-in-chess-png. 
		*	 Accessed 16 May 2021. 
		*/
		Pieces[PieceColor::black][PieceType::rook] = Texture::ImageTexture("res/textures/black rook.png");
		
		/*
		* black knight - https://imgbin.com/png/dz85yT9L/chess-piece-knight-king-chessboard-png
		* Klevstul. Chess Piece Black Knight. 5 Aug. 2017. IMGBIN, 5 Aug. 2017, imgbin.com/ 
		*	 png/dz85yT9L/chess-piece-knight-king-chessboard-png. Accessed 16 May 2021. 
		*/
		Pieces[PieceColor::black][PieceType::knight] = Texture::ImageTexture("res/textures/black knight.png");
		
		/*
		* black pawn - https://imgbin.com/png/qmZwxE9y/chess-piece-black-amp-white-pawn-white-and-black-in-chess-png
		* Nogonole1981. Chess Piece Black Pawn. 22 June 2017. IMGBIN, 22 June 2017, 
		*	imgbin.com/png/qmZwxE9y/ 
		*	chess-piece-black-amp-white-pawn-white-and-black-in-chess-png. Accessed 16 
		*	 May 2021. 
		*/
		Pieces[PieceColor::black][PieceType::pawn] = Texture::ImageTexture("res/textures/black pawn.png");
		
		
		/*
		* black queen - https://imgbin.com/png/CDMwcffv/chess-piece-queen-bishop-king-png
		* Paulus23101. Chess Piece Black Queen. 21 June 2018. IMGBIN, 21 June 2018, 
		*	 imgbin.com/png/CDMwcffv/chess-piece-queen-bishop-king-png. Accessed 16 May 
		*	 2021. 
		*/
		Pieces[PieceColor::black][PieceType::queen] = Texture::ImageTexture("res/textures/black queen.png");
		
		/*
		* black bishop - https://imgbin.com/png/TE7XKd60/chess-piece-bishop-knight-png 
		* Gympigeon59. Chess Piece Black Bishop. 29 May 2017. Chess Piece Black Bishop, 29 
		*	May 2017, imgbin.com/png/TE7XKd60/chess-piece-bishop-knight-png. Accessed 
		*	16 May 2021. 
		*/
		Pieces[PieceColor::black][PieceType::bishop] = Texture::ImageTexture("res/textures/black bishop.png");
		
		/*
		* black king - https://imgbin.com/png/Ft6mfY6Y/chess-piece-king-white-and-black-in-chess-png
		* GutoS. Chess Piece Black King. 3 Feb. 2017. IMGBIN, 3 Feb. 2017, imgbin.com/png/ 
		*	 Ft6mfY6Y/chess-piece-king-white-and-black-in-chess-png. Accessed 16 May 
		*	 2021. 
		*/
		Pieces[PieceColor::black][PieceType::king] = Texture::ImageTexture("res/textures/black king.png");
	}
	s_Instances++;

	m_mode = Mode::Select;
	m_currentMove = PieceColor::white;
	Init();
}

 void BoardScript::unhighlightBox()
 {
	 Sprite2DComponent& sprite = getComponent<Sprite2DComponent>();
	 if (selectedPosition.r < 0 || selectedPosition.r > 7 || selectedPosition.c < 0 || selectedPosition.c > 7)
		 return;
	 sprite.texture->paintColoredQuad((int)(selectedPosition.c * 800 / 8.0f), (int)((7 - selectedPosition.r) * 800 / 8.0f), (int)(800 / 8.0f), (int)(800 / 8.0f), BoardScript::BoardColors[(int)((selectedPosition.r + selectedPosition.c) % 2 == 0)]);

 }

  void BoardScript::onDestroy()  {
	 ClearBoard();
	 s_Instances--;
	 if (s_Instances == 0) {
		 for (int i = 0; i < 2; i++) {
			 for (int z = 0; z < 6; z++)
				 if (Pieces[i][z]) {
					 delete Pieces[i][z];
					 Pieces[i][z] = nullptr;
				 }
		 }
	 }
 }

  void BoardScript::highlightBox(int r, int c)
  {
	  Sprite2DComponent& sprite = getComponent<Sprite2DComponent>();
	  sprite.texture->paintColoredQuad((int)(c * 800 / 8.0f), (int)((7 - r) * 800 / 8.0f), (int)(800 / 8.0f), (int)(800 / 8.0f), BoardColors[2]);
  }

  std::tuple<int, int> BoardScript::getBox(float x, float y)
  {
	  TransformComponent& ts = getComponent<TransformComponent>();
	  int r = (int)(7 - (int)((y - ts.y + ts.y_scale / 2.0f) / ((float)(ts.y_scale / 8.0f))));
	  float c_float = (((float)(x - ts.x + ts.x_scale / 2.0f) / ((float)(ts.x_scale / 8.0f))));
	  int c = (int)c_float;
	  if (c_float < 0)
		  c = c-1;
	  return std::tuple<int, int>(r, c);
  }

