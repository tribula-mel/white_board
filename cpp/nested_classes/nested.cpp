class M {
public:
   void test(void) { };
};

class A {
  int x;
  M mel;

  class B { };

  class C {

    // The compiler cannot allow the following
    // declaration because A::B is private:
       B b;

public:
    int y;
    void f(A* p, int i) {

    // The compiler cannot allow the following
    // statement because A::x is private:
       p->x = i;

    }
    void f1(void) {

    // The compiler cannot allow the following
    // statement because A::x is private:
       m1();

    }
  };

  static void m1(void) {
	mel.test();
  }

  void g(C* p) {

    // The compiler cannot allow the following
    // statement because C::y is private:
       int z = p->y;
	z = z/2;
  }
};

int main() { }
