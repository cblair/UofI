from c4 import c4
from c4 import c4_2
from c4h import c4h
import time, math, random


class c4search:
	def __init__(self):
		self.c4temp = c4()
		self.c4h = c4h()
		#seconds
		self.maxd = 4 # 0 - n
		#for alpha_beta_timed
		self.time_limit = 30 
		self.total_turns = 1

	def times_up(self,depth,bfactor):
		#always allow depth 0 analysis
		if depth == 0:
			return False
		if self.maxd_timed == -1: #not inited
			rbfactor = self.total_turns / bfactor #reduced branching factor
			bfactor = bfactor - rbfactor
			pottime = math.pow(depth,bfactor) * self.run_time
			#pottime = math.pow(bfactor,depth) * self.run_time
			if (pottime >= self.time_limit):
				self.maxd_timed = depth
				print "Max depth set to %d" % (self.maxd_timed + 1) #starts at 0
				print " potential full search time: %f" % pottime 
				return True
		elif depth >= self.maxd_timed:
			return True
		#emergency time check
		if (self.run_time + 1 > self.time_limit):
			return True
		return False


	def best_move(self, c4_orig,depth):
		self.c4temp.board = c4_orig.copy_board()
		self.c4temp.mode = c4_orig.mode
		self.c4temp.turn = c4_orig.turn
		stime = time.time()
		if self.c4temp.mode == "ai_test":
			col = self.ai_test(depth)[1]
		elif self.c4temp.mode == "minmax":
			col = self.minmax(self.c4temp,depth)[1] #return just the col
		elif self.c4temp.mode == "alpha_beta":
			col = self.alpha_beta(self.c4temp,depth)[1] #return just the col
		elif self.c4temp.mode == "alpha_beta_timed":
			self.maxd_timed = -1	 
			self.start_time = time.time()
			col = self.alpha_beta_timed(self.c4temp,depth)[1] #return just the col
			self.maxd = self.maxd_timed
			self.total_turns = self.total_turns + 2
		etime = time.time()
		rtime = etime - stime
		print "########################################"
		print "Best col = %d" % col
		print " search took %f seconds" % rtime
		print " max depth was %d" % (self.maxd + 1) # 0 - n
		print
		return col

	def max_h(self, potgames):
		max = potgames[3].hval
		maxi = 3 #because middle is best?
		block = False
		for i in range(0,self.c4temp.xmax):
			if potgames[i].hval > max:
				max = potgames[i].hval
				maxi = i
			if potgames[i].hval < 0:
				block = True
		#if col 3 is not playable, select randomly from all
		if (maxi == 3) and (potgames[3].avail_colrow(3) == -1) and (not block):
			x = [i for i in range(potgames[3].xmax)]
			random.shuffle(x)
			for i in x:
				if potgames[i].avail_colrow(i) != -1:
					max = potgames[i].hval
					maxi = i
					return (max,maxi)
		return (max,maxi)

	def min_h(self, potgames):
		min = potgames[0].hval
		mini = 3 #because the side is worst?
		for i in range(0,self.c4temp.xmax):
			if potgames[i].hval < min:
				min = potgames[i].hval
				mini = i
		return (min,mini)

	def ai_test(self,depth):
		self.c4temp.potgames = []
		#set turn ahead to current player
		turn = (self.c4temp.turn + 1) % 2
		mode = self.c4temp.mode
		for i in range(0,self.c4temp.xmax):
			stime = time.time()
			self.c4temp.potgames.append(c4())
			self.c4temp.potgames[i].board = self.c4temp.copy_board()
			self.c4temp.potgames[i].turn = turn
			self.c4temp.potgames[i].mode = mode
			self.c4temp.potgames[i].hval = self.c4h.h1(self.c4temp.potgames[i],i)
			etime = time.time()
			rtime = etime - stime
			print " Board col %d, rtime = %f" % (i,rtime)
			#self.c4temp.potgames[i].print_board()
			#print "  hval = %d" % (self.c4temp.potgames[i].hval)
			#print ".",
		print
		print "Best col %d" % self.max_h(self.c4temp.potgames)[1]
		return(self.max_h(self.c4temp.potgames))


	def minmax(self,c4_orig,depth):
		stime = time.time()
		c4temp = c4()
		self.c4h = c4h()
		c4temp.board = c4_orig.copy_board()
		c4temp.mode = c4_orig.mode
		c4temp.turn = c4_orig.turn
		c4temp.potgames = []
		turn = c4temp.turn
		mode = c4temp.mode
		for i in range(0,c4temp.xmax):
			c4temp.potgames.append(c4())
			c4temp.potgames[i].board = c4temp.copy_board()
			c4temp.potgames[i].turn = turn
			c4temp.potgames[i].mode = mode
			c4temp.potgames[i].sim = True
			if c4temp.potgames[i].play_piece(i):
				c4temp.potgames[i].turn = (turn + 1) % 2
				if depth < self.maxd and not c4temp.potgames[i].is_winner():
					c4temp.potgames[i].hval = self.minmax(c4temp.potgames[i],depth + 1)[0]
				else:
					c4temp.potgames[i].hval = self.c4h.h2(c4temp.potgames[i],i,(turn + 1) == c4temp.red)
				if depth == 0:	
					print "Board %d, depth %d, turn: %d".rjust(27 + depth + 1) % (i,depth, c4temp.potgames[i].turn + 1)
					print " hval: %d".rjust(9 + depth) % (c4temp.potgames[i].hval)
					#c4temp.potgames[i].print_board()
			else: #if not potgame[i].play_piece(i)
				if (turn + 1) == c4temp.black:
					c4temp.potgames[i].hval = -2
				elif (turn + 1) == c4temp.red:
					c4temp.potgames[i].hval = 2
					
		etime = time.time()
		rtime = etime - stime
		#print "Depth %d" % (depth)
		#print "run time: %f" % (rtime)
		if (turn + 1) == c4temp.black:
			pass
			#print "My move, max: %d" % self.max_h(c4temp.potgames)[1]
			return(self.max_h(c4temp.potgames))
		elif (turn + 1) == c4temp.red:
			pass
			#print "Opp move, max: %d" % self.max_h(c4temp.potgames)[1]
			return(self.min_h(c4temp.potgames))


	def alpha_beta(self,c4_orig,depth):
		stime = time.time()
		c4temp = c4()
		self.c4h = c4h()
		c4temp.board = c4_orig.copy_board()
		c4temp.mode = c4_orig.mode
		c4temp.turn = c4_orig.turn
		c4temp.potgames = []
		turn = c4temp.turn
		mode = c4temp.mode
		for i in range(0,c4temp.xmax):
			c4temp.potgames.append(c4())
			c4temp.potgames[i].board = c4temp.copy_board()
			c4temp.potgames[i].turn = turn
			c4temp.potgames[i].mode = mode
			c4temp.potgames[i].sim = True
			if c4temp.potgames[i].play_piece(i):
				c4temp.potgames[i].turn = (turn + 1) % 2
				if depth < self.maxd and not c4temp.potgames[i].is_winner():
					c4temp.potgames[i].hval = self.alpha_beta(c4temp.potgames[i],depth + 1)[0]
				else:
					c4temp.potgames[i].hval = self.c4h.h2(c4temp.potgames[i],i,(turn + 1) == c4temp.red)
				if depth == 0:	
					print "Board %d, depth %d, turn: %d".rjust(27 + depth + 1) % (i,depth, c4temp.potgames[i].turn + 1)
					print " hval: %d".rjust(9 + depth) % (c4temp.potgames[i].hval)
					#c4temp.potgames[i].print_board()
				#pruning
				if (c4temp.potgames[i].hval == 1
					and (turn + 1) == c4temp.black): #my move
					return (1,i)
				if (c4temp.potgames[i].hval == -1 
					and (turn + 1) == c4temp.red): #opp move
					return (-1,i)
				#end pruning
			else: #if not potgame[i].play_piece(i)
				if (turn + 1) == c4temp.black:
					c4temp.potgames[i].hval = -2
				elif (turn + 1) == c4temp.red:
					c4temp.potgames[i].hval = 2
					
		etime = time.time()
		rtime = etime - stime
		#print "Depth %d" % (depth)
		#print "run time: %f" % (rtime)
		if (turn + 1) == c4temp.black:
			pass
			#print "My move, max: %d" % self.max_h(c4temp.potgames)[1]
			return(self.max_h(c4temp.potgames))
		elif (turn + 1) == c4temp.red:
			pass
			#print "Opp move, max: %d" % self.max_h(c4temp.potgames)[1]
			return(self.min_h(c4temp.potgames))


	def alpha_beta_timed(self,c4_orig,depth):
		stime = time.time()
		c4temp = c4()
		self.c4h = c4h()
		c4temp.board = c4_orig.copy_board()
		c4temp.mode = c4_orig.mode
		c4temp.turn = c4_orig.turn
		c4temp.potgames = []
		turn = c4temp.turn
		mode = c4temp.mode
		for i in range(0,c4temp.xmax):
		#x = [i for i in range(c4temp.xmax)]
		#random.shuffle(x)
		#for i2 in x:
			c4temp.potgames.append(c4())
			i = len(c4temp.potgames) - 1
			c4temp.potgames[i].board = c4temp.copy_board()
			c4temp.potgames[i].turn = turn
			c4temp.potgames[i].mode = mode
			c4temp.potgames[i].sim = True
			if c4temp.potgames[i].play_piece(i):
				c4temp.potgames[i].turn = (turn + 1) % 2
				self.run_time = time.time() - self.start_time
				#if depth < self.maxd and not c4temp.potgames[i].is_winner():
				#run_time gets a little ahead, this comparison works
				if (not self.times_up(depth,c4temp.xmax)
					#and depth < self.maxd
					and not c4temp.potgames[i].is_winner()):
					c4temp.potgames[i].hval = self.alpha_beta_timed(c4temp.potgames[i],depth + 1)[0]
				else:
					c4temp.potgames[i].hval = self.c4h.h2(c4temp.potgames[i],i,(turn + 1) == c4temp.red)
				if depth == 0:	
					print "Board %d, depth %d, turn: %d, time: %f".rjust(37 + depth + 1) % (i,depth, c4temp.potgames[i].turn + 1,self.run_time)
					print " hval: %d".rjust(9 + depth) % (c4temp.potgames[i].hval)
					#c4temp.potgames[i].print_board()
				#pruning
				if (c4temp.potgames[i].hval == 1
					and (turn + 1) == c4temp.black): #my move
					return (1,i)
				if (c4temp.potgames[i].hval == -1 
					and (turn + 1) == c4temp.red): #opp move
					return (-1,i)
				#end pruning
			else: #if not potgame[i].play_piece(i)
				if (turn + 1) == c4temp.black:
					c4temp.potgames[i].hval = -2
				elif (turn + 1) == c4temp.red:
					c4temp.potgames[i].hval = 2					
		etime = time.time()
		rtime = etime - stime
		#print "Depth %d" % (depth)
		#print "run time: %f" % (rtime)
		if (turn + 1) == c4temp.black:
			pass
			#print "My move, max: %d" % self.max_h(c4temp.potgames)[1]
			return(self.max_h(c4temp.potgames))
		elif (turn + 1) == c4temp.red:
			pass
			#print "Opp move, max: %d" % self.max_h(c4temp.potgames)[1]
			return(self.min_h(c4temp.potgames))


	def sel_eval(self):
		pass

	def max_depth(self):
		pass
