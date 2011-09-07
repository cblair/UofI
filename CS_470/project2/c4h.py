from c4 import c4

class c4h:
	def __init__(self):
		pass

	def is_vert_winnable(self,potgame,x):
		for y in range(0,potgame.ymax):
			if (potgame.board[x][y] == potgame.empty
				and y >= 2):
				return True
		return False

	def h1(self,potgame_orig,x):
		potgame = c4()
		potgame.board = potgame_orig.copy_board()
		potgame.turn = potgame_orig.turn
		potgame.mode = potgame_orig.mode		
		#make parallel copy for block detection
		potgame_block = c4()
		potgame_block.board = potgame.copy_board()
		potgame_block.turn = potgame.turn

		#win detection
		#potgame.turn = (potgame.turn + 1) % 2
		if not potgame.play_piece(x):
			return -1
		if potgame.is_winner():
			return 300
		potgame.turn = (potgame.turn + 1) % 2
		#end win detection
		#block detection
		potgame_block.turn = (potgame.turn + 1) % 2
		if not potgame_block.play_piece(x):
			 return -1
		if potgame_block.is_winner():
			return 300
		
		#otherwise, we need to do some stats
		totalScore = 0 #total probability of game win
		emptyScore = 1
		pieceScore = 3
		piece = potgame.turn + 1
		empty = potgame.empty
		opp = ((potgame.turn + 1) % 2) + 1
		#find y - pos of played piece
		for y in range(0,potgame.ymax):
			if potgame.board[x][y] == piece:
				break
			if potgame.board[x][y] == opp:
				y = y - 1
				break
		#get vert. score
		# y2 - bottom-most, not-seperated piece
		y2 = y
		while(y2 < potgame.ymax and potgame.board[x][y2] == piece):
				y2 = y2 - 1
		if (y2 - 1 >= 0
			and y2 - 2 >= 0
			and y2 - 3 >= 0
			):
			if potgame.board[x][y2 - 1] == empty:
				totalScore = totalScore + emptyScore
			elif potgame.board[x][y2 - 1] == piece:
				totalScore = totalScore + pieceScore
			if potgame.board[x][y2 - 2] == empty:
				totalScore = totalScore + emptyScore
			elif potgame.board[x][y2 - 2] == piece:
				totalScore = totalScore + pieceScore
			if potgame.board[x][y2 - 3] == empty:
				totalScore = totalScore + emptyScore
			elif potgame.board[x][y2 - 3] == piece:
				totalScore = totalScore + pieceScore
		#get hor. score
		#left score
		if x < 3:
			pass
		else: 
			if (potgame.board[x-1][y] == opp
				or potgame.board[x-2][y] == opp
				or potgame.board[x-3][y] == opp
				):
				pass
			else:
				if potgame.board[x-1][y] == piece:
					totalScore = totalScore + pieceScore
				elif potgame.board[x-1][y] == empty:
					totalScore = totalScore + emptyScore
				if potgame.board[x-2][y] == piece:
					totalScore = totalScore + pieceScore
				elif potgame.board[x-2][y] == empty:
					totalScore = totalScore + emptyScore
				if potgame.board[x-3][y] == piece:
					totalScore = totalScore + pieceScore
				elif potgame.board[x-3][y] == empty:
					totalScore = totalScore + emptyScore
		#right score
		if x > potgame.xmax - 4 :
			pass
		else:
			if (potgame.board[x-1][y] == opp
				or potgame.board[x-2][y] == opp
				or potgame.board[x-3][y] == opp
				):
				pass
			else:
				if potgame.board[x-1][y] == piece:
					totalScore = totalScore + pieceScore
				elif potgame.board[x-1][y] == empty:
					totalScore = totalScore + emptyScore
				if potgame.board[x-2][y] == piece:
					totalScore = totalScore + pieceScore
				elif potgame.board[x-2][y] == empty:
					totalScore = totalScore + emptyScore
				if potgame.board[x-3][y] == piece:
					totalScore = totalScore + pieceScore
				elif potgame.board[x-3][y] == empty:
					totalScore = totalScore + emptyScore

		#return(random.randint(0,6))
		return(totalScore)

	def h2(self,potgame,x,is_opp):
		#potgame = c4()
		#potgame.board = potgame_orig.copy_board()
		#potgame.turn = potgame_orig.turn
		#potgame.mode = potgame_orig.mode		

		#win detection
		if potgame.is_winner():
			if is_opp:
				return -1
			elif not is_opp:
				return 1
		else:
			return 0
