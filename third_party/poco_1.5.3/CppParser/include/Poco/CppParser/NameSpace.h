//
// NameSpace.h
//
// $Id: //poco/1.4/CppParser/include/Poco/CppParser/NameSpace.h#1 $
//
// Library: CppParser
// Package: SymbolTable
// Module:  NameSpace
//
// Definition of the NameSpace class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef CppParser_NameSpace_INCLUDED
#define CppParser_NameSpace_INCLUDED


#include "Poco/CppParser/CppParser.h"
#include "Poco/CppParser/Symbol.h"
#include <map>
#include <vector>
#include <set>


namespace Poco {
namespace CppParser {


class CppParser_API NameSpace: public Symbol
	/// This class represents a namespace.
{
public:
	typedef std::multimap<std::string, Symbol*> SymbolTable;
	typedef SymbolTable::const_iterator Iterator;
	typedef std::map<std::string, std::string> AliasMap;
	typedef std::vector<std::string> NameSpaceVec;
	
	NameSpace();
		/// Creates the NameSpace.

	NameSpace(const std::string& name, NameSpace* pNameSpace = 0);
		/// Creates the NameSpace.

	~NameSpace();
		/// Destroys the NameSpace.
	
	void addSymbol(Symbol* pSymbol);
		/// Adds a symbol to the namespace.
		
	void importSymbol(const std::string& fullName);
		/// Imports a symbol from another namespace (using <symbol>).
		
	void importNameSpace(const std::string& nameSpace);
		/// Imports a namespace (using namespace <namespace>).
	
	Iterator begin() const;
		/// Returns an iterator for iterating over the NameSpace's Symbol's.

	Iterator end() const;
		/// Returns an iterator for iterating over the NameSpace's Symbol's.
	
	Symbol* lookup(const std::string& name) const;
		/// Looks up the given name in the symbol table
		/// and returns the corresponsing symbol, or null
		/// if no symbol can be found. The name can include
		/// a namespace.
		
	static NameSpace* root();
		/// Returns the root namespace. Never delete this one!
	
	void nameSpaces(SymbolTable& table) const;
		/// Fills the symbol table with all namespaces.
		
	void typeDefs(SymbolTable& table) const;
		/// Fills the symbol table with all type definitions.
		
	void enums(SymbolTable& table) const;
		/// Fills the symbol table with all enums.
		
	void classes(SymbolTable& table) const;
		/// Fills the symbol table with all classes and structs.
		
	void functions(SymbolTable& table) const;
		/// Fills the symbol table with all functions.
		
	void variables(SymbolTable& table) const;
		/// Fills the symbol table with all variables.
		
	const AliasMap& importedSymbols() const;
		/// Returns a const reference to a SymbolTable containing all
		/// imported symbols.
		
	const NameSpaceVec& importedNameSpaces() const;
		/// Returns a vector containing all imported namespaces.

	Symbol::Kind kind() const;
	std::string toString() const;

private:
	Symbol* lookup(const std::string& name, std::set<const NameSpace*>& alreadyVisited) const;
		/// Looks up the given name in the symbol table
		/// and returns the corresponsing symbol, or null
		/// if no symbol can be found. The name can include
		/// a namespace.

protected:
	void extract(Symbol::Kind kind, SymbolTable& table) const;
	static void splitName(const std::string& name, std::string& head, std::string& tail);

private:
	SymbolTable _symbols;
	AliasMap _importedSymbols;
	NameSpaceVec _importedNameSpaces;
};


//
// inlines
//
inline const NameSpace::AliasMap& NameSpace::importedSymbols() const
{
	return _importedSymbols;
}

	
inline const NameSpace::NameSpaceVec& NameSpace::importedNameSpaces() const
{
	return _importedNameSpaces;
}


} } // namespace Poco::CppParser


#endif // CppParser_NameSpace_INCLUDED
