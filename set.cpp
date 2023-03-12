
#include "set.h"

void print( std::ostream& out, const treenode* n, size_t indent )
{

   for( size_t i = 0; i != indent; ++ i )
      out << "|  "; 
   if(n)
   {
      out << ( n -> val ) << "\n";
      print( out, n -> left, indent + 1 );
      print( out, n -> right, indent + 1 ); 
   }
   else
      out << "#\n"; 
}


void checksorted( 
          treenode* n, 
          const std::string* lowerbound, 
          const std::string* upperbound )
{
   while(n) 
   {
      if( lowerbound && !before( *lowerbound, n -> val ))
      {
         std::cout << "value " << ( n -> val );
         std::cout << " is not above lower bound " << *lowerbound << "\n";
         std::abort( );
      }

      if( upperbound && !before( n -> val, *upperbound ))
      {
         std::cout << "value " << ( n -> val );
         std::cout << "is not below upperbound " << *upperbound << "\n";
         std::abort( );
      }

      checksorted( n -> left, lowerbound, &( n -> val ));
      lowerbound = & ( n -> val );
      n = n -> right;
   }
} 


// Used by copy constructor and assignment:

void writecopy( treenode** to, treenode* from )
{
   while( from )
   {
      *to = new treenode{ from -> val };

      writecopy( & (*to) -> left, from -> left );
      to = & (*to) -> right;
      from = from -> right;
   }
}

// Used by destructor:

void deallocate( treenode* n )
{
   while(n)
   {
      deallocate( n -> left );
      treenode* bye = n;
      n = n -> right;
      delete bye;
   }
}



size_t log_base2(size_t s) {
	size_t ans = 0;
	if (s <= 1) return ans;
	while (s >= 1) {
		s = s / 2;
		ans++;
	}
	return (ans - 1);
}

// From previous task:


bool before(const std::string& s1, const std::string& s2) {
	size_t len, i = 0;
	if (s1.size() <= s2.size()) len = s1.size();
	else len = s2.size();
	while (i <= len) {
		if ((char)tolower(s1[i]) < (char)tolower(s2[i])) return true;
		else if ((char)tolower(s1[i]) > (char)tolower(s2[i])) return false;
		i++;
	}
	if (s1.size() < s2.size()) return true;
	else return false;
}


bool equal(const std::string& s1, const std::string& s2) {
	if (before(s1, s2) == true && before(s2, s1) == true) return true;
	else return false;
}
// Must be written for task 5(part 2) :


const treenode* find( const treenode* n, const std::string& el ){
	if (n == nullptr) return n;
	//if (equal(n -> val, el)) return n;
	else if ( before(n -> val, el)) return find ( n -> right, el );
	else if (before(el, n -> val)) return find ( n -> left, el );
	else return n;
}
   // Write this one first.

treenode** find( treenode** n, const std::string& el ){
	if (*n == nullptr) return n;
	//if ( (*n)->val == el) return n; 
	else if (before((*n) -> val, el)) return find (&((*n)->right), el);
	else if (before(el, (*n)->val)) return find (&((*n)->left), el);
	else return n;
}
   // Modify the other find into this one, also have a look at
   // stackscan4.pdf


// Insert n at the right most position in into: 

void rightinsert(treenode** into, treenode* n) {
	while ((*into)->right != nullptr)
		into = &((*into)->right);
	(*into)->right = n;
}


size_t size(const treenode* n) {
	size_t left, right = 0;
	if (n == nullptr) return 0;
	else {
		left = size(n->left);
		right = size(n->right);
		return right + left + 1;
	}
}

size_t height(const treenode* n) {
	if (n == nullptr) return 0;
	else if (n->left != nullptr || n->right != nullptr) {
		if (height(n->right) > height(n->left)) return height(n->right) + 1;
		else return height(n->left) + 1;
	}
	return 0;
}

 
bool set::insert(const std::string& el) {
	treenode **ins = find(&tr, el);
	if (*ins == nullptr) {
		*ins = new treenode(el);
		return true;
	}
	return false;
}

bool set::contains( const std::string& el ) const 
{
   return find( tr, el ) != nullptr; 
}


bool set::remove(const std::string& el) {
	treenode** root = find(&tr, el);

	if ((*root) == nullptr) return false;
	if ((*root)->left == nullptr)
	{
		treenode *bye = *root;
		*root = (*root)->right;
		delete bye;
		return true;
	}
	else if ((*root)->right == nullptr)
	{
		treenode *bye = *root;
		*root = (*root)->left;
		delete bye;
		return true;
	}
	else if ((*root)->right == nullptr && (*root)->left == nullptr)
	{
		delete *root;
		*root = nullptr;
		return true;
	}
	else {
		rightinsert(&((*root)->left), (*root)->right);
		treenode* bye = *root;
		*root = (*root)->left;
		delete bye;
		return true;
	}
}


void set::checksorted( ) const
{
   ::checksorted( tr, nullptr, nullptr );
      // :: are needed because the other checksorted is not in the class. 
}


std::ostream& set::print( size_t indent, std::ostream& out ) const
{
   ::print( out, tr, indent );
   return out;
}


