#include "DerivativeVisitor.h"




DerivativeVisitor::DerivativeVisitor()
{
}

template<typename T>
void DerivativeVisitor::VisitAddHelper(T* n)
{
	if (n != nullptr) {
		auto derivative = std::make_shared<DerivativeVisitor>();
		if (n->left.get() != nullptr && n->right.get() != nullptr) {
			n->left->Accept(derivative.get());
			AtomPtr a = derivative.get()->get();
			n->right->Accept(derivative.get());
			AtomPtr b = derivative.get()->get();
			root = std::make_shared<T>(a, b);
			return;
		}
		if (n->left.get() != nullptr) {
			n->left->Accept(derivative.get());
			root = derivative.get()->get();
			return;
		}
		if (n->right.get() != nullptr) {
			n->right->Accept(derivative.get());
			root = derivative.get()->get();
			return;
		}
	}
}

void DerivativeVisitor::VisitConst(Const* n)
{
	if(n != nullptr) root = std::make_shared<Const>(0);
}

void DerivativeVisitor::VisitX(X* n)
{
	if(n != nullptr) root = std::make_shared<Const>(1);
}

void DerivativeVisitor::VisitAdd(Add* n)
{
	VisitAddHelper<Add>(n);
}

void DerivativeVisitor::VisitMinus(Minus* n)
{
	VisitAddHelper<Minus>(n);
}

void DerivativeVisitor::VisitTimes(Times* n)
{
	// a'b + ab' = apb + abp
	if (n != nullptr) {
		auto derivative = std::make_shared<DerivativeVisitor>();
		if (n->left.get() != nullptr && n->right.get() != nullptr) {
			n->left->Accept(derivative.get());
			AtomPtr ap = derivative.get()->get();
			AtomPtr b = n->right;
			n->right->Accept(derivative.get());
			AtomPtr a = n->left;
			AtomPtr bp = derivative.get()->get();
			root = std::make_shared<Add>(std::make_shared<Times>(ap,b), std::make_shared<Times>(a,bp));
			return;
		}
		if (n->left.get() != nullptr) {
			n->left->Accept(derivative.get());
			root = derivative.get()->get();
			return;
		}
		if (n->right.get() != nullptr) {
			n->right->Accept(derivative.get());
			root = derivative.get()->get();
			return;
		}
	}
}

void DerivativeVisitor::VisitDivide(Divide* n)
{
}

void DerivativeVisitor::VisitExp(Exp* n)
{
}


AtomPtr DerivativeVisitor::get()
{
	return root;
}

DerivativeVisitor::~DerivativeVisitor()
{
}