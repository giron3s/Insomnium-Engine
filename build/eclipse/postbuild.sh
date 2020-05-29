10:11:16 **** Incremental Build of configuration Debug for project framework ****
make all 
Building file: ../graphic/gui/common.cpp
Invoking: GCC C++ Compiler
g++ -std=c++0x -DGLEW_STATIC -DJSON_IS_AMALGAMATION -D_DEBUG -I/usr/include/GL -I/usr/include/glm -I/usr/include/qt4 -I/usr/include/qt4/Qt -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtCore -I/usr/include/SDL2 -I/usr/include/jsoncpp -I"/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework" -I"/home/mgironesd/workspace/Workspace_Private/interviewtest/src/extern/tinyobjloader" -I"/home/mgironesd/workspace/Workspace_Private/interviewtest/src/extern/nanovg" -O0 -g3 -Wall -c -fmessage-length=0 -Wno-format-security -v -fPIC -MMD -MP -MF"graphic/gui/common.d" -MT"graphic/gui/common.o" -o "graphic/gui/common.o" "../graphic/gui/common.cpp"
Using built-in specs.
COLLECT_GCC=g++
Target: x86_64-linux-gnu
Configured with: ../src/configure -v --with-pkgversion='Debian 4.9.2-10' --with-bugurl=file:///usr/share/doc/gcc-4.9/README.Bugs --enable-languages=c,c++,java,go,d,fortran,objc,obj-c++ --prefix=/usr --program-suffix=-4.9 --enable-shared --enable-linker-build-id --libexecdir=/usr/lib --without-included-gettext --enable-threads=posix --with-gxx-include-dir=/usr/include/c++/4.9 --libdir=/usr/lib --enable-nls --with-sysroot=/ --enable-clocale=gnu --enable-libstdcxx-debug --enable-libstdcxx-time=yes --enable-gnu-unique-object --disable-vtable-verify --enable-plugin --with-system-zlib --disable-browser-plugin --enable-java-awt=gtk --enable-gtk-cairo --with-java-home=/usr/lib/jvm/java-1.5.0-gcj-4.9-amd64/jre --enable-java-home --with-jvm-root-dir=/usr/lib/jvm/java-1.5.0-gcj-4.9-amd64 --with-jvm-jar-dir=/usr/lib/jvm-exports/java-1.5.0-gcj-4.9-amd64 --with-arch-directory=amd64 --with-ecj-jar=/usr/share/java/eclipse-ecj.jar --enable-objc-gc --enable-multiarch --with-arch-32=i586 --with-abi=m64 --with-multilib-list=m32,m64,mx32 --enable-multilib --with-tune=generic --enable-checking=release --build=x86_64-linux-gnu --host=x86_64-linux-gnu --target=x86_64-linux-gnu
Thread model: posix
gcc version 4.9.2 (Debian 4.9.2-10) 
COLLECT_GCC_OPTIONS='-std=c++11' '-D' 'GLEW_STATIC' '-D' 'JSON_IS_AMALGAMATION' '-D' '_DEBUG' '-I' '/usr/include/GL' '-I' '/usr/include/glm' '-I' '/usr/include/qt4' '-I' '/usr/include/qt4/Qt' '-I' '/usr/include/qt4/QtGui' '-I' '/usr/include/qt4/QtCore' '-I' '/usr/include/SDL2' '-I' '/usr/include/jsoncpp' '-I' '/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework' '-I' '/home/mgironesd/workspace/Workspace_Private/interviewtest/src/extern/tinyobjloader' '-I' '/home/mgironesd/workspace/Workspace_Private/interviewtest/src/extern/nanovg' '-O0' '-g3' '-Wall' '-c' '-fmessage-length=0' '-Wno-format-security' '-v' '-fPIC' '-MMD' '-MP' '-MF' 'graphic/gui/common.d' '-MT' 'graphic/gui/common.o' '-o' 'graphic/gui/common.o' '-shared-libgcc' '-mtune=generic' '-march=x86-64'
 /usr/lib/gcc/x86_64-linux-gnu/4.9/cc1plus -quiet -v -I /usr/include/GL -I /usr/include/glm -I /usr/include/qt4 -I /usr/include/qt4/Qt -I /usr/include/qt4/QtGui -I /usr/include/qt4/QtCore -I /usr/include/SDL2 -I /usr/include/jsoncpp -I /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework -I /home/mgironesd/workspace/Workspace_Private/interviewtest/src/extern/tinyobjloader -I /home/mgironesd/workspace/Workspace_Private/interviewtest/src/extern/nanovg -imultiarch x86_64-linux-gnu -MMD graphic/gui/common.d -MF graphic/gui/common.d -MP -MT graphic/gui/common.o -dD -D_GNU_SOURCE -D GLEW_STATIC -D JSON_IS_AMALGAMATION -D _DEBUG ../graphic/gui/common.cpp -quiet -dumpbase common.cpp -mtune=generic -march=x86-64 -auxbase-strip graphic/gui/common.o -g3 -O0 -Wall -Wno-format-security -std=c++11 -version -fmessage-length=0 -fPIC -o /tmp/ccJhxU83.s
GNU C++ (Debian 4.9.2-10) version 4.9.2 (x86_64-linux-gnu)
	compiled by GNU C version 4.9.2, GMP version 6.0.0, MPFR version 3.1.2-p3, MPC version 1.0.2
GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
ignoring duplicate directory "/usr/include/x86_64-linux-gnu/c++/4.9"
ignoring nonexistent directory "/usr/local/include/x86_64-linux-gnu"
ignoring nonexistent directory "/usr/lib/gcc/x86_64-linux-gnu/4.9/../../../../x86_64-linux-gnu/include"
#include "..." search starts here:
#include <...> search starts here:
 /usr/include/GL
 /usr/include/glm
 /usr/include/qt4
 /usr/include/qt4/Qt
 /usr/include/qt4/QtGui
 /usr/include/qt4/QtCore
 /usr/include/SDL2
 /usr/include/jsoncpp
 /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework
 /home/mgironesd/workspace/Workspace_Private/interviewtest/src/extern/tinyobjloader
 /home/mgironesd/workspace/Workspace_Private/interviewtest/src/extern/nanovg
 /usr/include/c++/4.9
 /usr/include/x86_64-linux-gnu/c++/4.9
 /usr/include/c++/4.9/backward
 /usr/lib/gcc/x86_64-linux-gnu/4.9/include
 /usr/local/include
 /usr/lib/gcc/x86_64-linux-gnu/4.9/include-fixed
 /usr/include/x86_64-linux-gnu
 /usr/include
End of search list.
GNU C++ (Debian 4.9.2-10) version 4.9.2 (x86_64-linux-gnu)
	compiled by GNU C version 4.9.2, GMP version 6.0.0, MPFR version 3.1.2-p3, MPC version 1.0.2
GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
Compiler executable checksum: ff85870e740eb08c48a56c2c170390f8
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h: In member function ‘float& Framework::GUI::Color::r()’:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:256:40: error: invalid initialization of reference of type ‘float&’ from expression of type ‘int’
             float &r() { return mColor.r; }
                                        ^
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h: In member function ‘const float& Framework::GUI::Color::r() const’:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:258:52: warning: returning reference to temporary [-Wreturn-local-addr]
             const float &r() const { return mColor.r; }
                                                    ^
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h: In member function ‘float& Framework::GUI::Color::g()’:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:260:40: error: invalid initialization of reference of type ‘float&’ from expression of type ‘int’
             float &g() { return mColor.g; }
                                        ^
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h: In member function ‘const float& Framework::GUI::Color::g() const’:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:262:52: warning: returning reference to temporary [-Wreturn-local-addr]
             const float &g() const { return mColor.g; }
                                                    ^
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h: In member function ‘float& Framework::GUI::Color::b()’:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:264:40: error: invalid initialization of reference of type ‘float&’ from expression of type ‘int’
             float &b() { return mColor.b; }
                                        ^
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h: In member function ‘const float& Framework::GUI::Color::b() const’:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:266:53: warning: returning reference to temporary [-Wreturn-local-addr]
             const float &b() const { return  mColor.b; }
                                                     ^
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h: In member function ‘float& Framework::GUI::Color::a()’:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:268:40: error: invalid initialization of reference of type ‘float&’ from expression of type ‘int’
             float &a() { return mColor.a; }
                                        ^
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h: In member function ‘const float& Framework::GUI::Color::a() const’:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:270:53: warning: returning reference to temporary [-Wreturn-local-addr]
             const float &a() const { return  mColor.a; }
                                                     ^
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h: In member function ‘Framework::GUI::Color Framework::GUI::Color::contrastingColor() const’:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:49: error: no match for ‘operator*’ (operand types are ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ and ‘const ivec4 {aka const glm::detail::tvec4<int, (glm::precision)0u>}’)
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                 ^
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:49: note: candidates are:
In file included from /usr/include/glm/detail/type_vec1.hpp:274:0,
                 from /usr/include/glm/detail/_vectorize.hpp:32,
                 from /usr/include/glm/detail/func_trigonometric.inl:29,
                 from /usr/include/glm/detail/func_trigonometric.hpp:199,
                 from /usr/include/glm/trigonometric.hpp:32,
                 from /usr/include/glm/glm.hpp:108,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_vec1.inl:522:33: note: template<class T, glm::precision P> glm::detail::tvec1<T, P> glm::detail::operator*(const glm::detail::tvec1<T, P>&, const glm::detail::tvec1<T, P>&)
  GLM_FUNC_QUALIFIER tvec1<T, P> operator*
                                 ^
/usr/include/glm/detail/type_vec1.inl:522:33: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tvec1<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_vec1.hpp:274:0,
                 from /usr/include/glm/detail/_vectorize.hpp:32,
                 from /usr/include/glm/detail/func_trigonometric.inl:29,
                 from /usr/include/glm/detail/func_trigonometric.hpp:199,
                 from /usr/include/glm/trigonometric.hpp:32,
                 from /usr/include/glm/glm.hpp:108,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_vec1.inl:511:33: note: template<class T, glm::precision P> glm::detail::tvec1<T, P> glm::detail::operator*(const T&, const glm::detail::tvec1<T, P>&)
  GLM_FUNC_QUALIFIER tvec1<T, P> operator*
                                 ^
/usr/include/glm/detail/type_vec1.inl:511:33: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘const ivec4 {aka const glm::detail::tvec4<int, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tvec1<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_vec1.hpp:274:0,
                 from /usr/include/glm/detail/_vectorize.hpp:32,
                 from /usr/include/glm/detail/func_trigonometric.inl:29,
                 from /usr/include/glm/detail/func_trigonometric.hpp:199,
                 from /usr/include/glm/trigonometric.hpp:32,
                 from /usr/include/glm/glm.hpp:108,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_vec1.inl:500:33: note: template<class T, glm::precision P> glm::detail::tvec1<T, P> glm::detail::operator*(const glm::detail::tvec1<T, P>&, const T&)
  GLM_FUNC_QUALIFIER tvec1<T, P> operator*
                                 ^
/usr/include/glm/detail/type_vec1.inl:500:33: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tvec1<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat4x4.hpp:257:0,
                 from /usr/include/glm/mat4x4.hpp:32,
                 from /usr/include/glm/glm.hpp:106,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat4x4.inl:753:35: note: template<class T, glm::precision P> glm::detail::tmat4x4<T, P> glm::detail::operator*(const glm::detail::tmat4x4<T, P>&, const glm::detail::tmat4x4<T, P>&)
  GLM_FUNC_QUALIFIER tmat4x4<T, P> operator*
                                   ^
/usr/include/glm/detail/type_mat4x4.inl:753:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat4x4<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat4x4.hpp:257:0,
                 from /usr/include/glm/mat4x4.hpp:32,
                 from /usr/include/glm/glm.hpp:106,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat4x4.inl:731:35: note: template<class T, glm::precision P> glm::detail::tmat3x4<T, P> glm::detail::operator*(const glm::detail::tmat4x4<T, P>&, const glm::detail::tmat3x4<T, P>&)
  GLM_FUNC_QUALIFIER tmat3x4<T, P> operator*
                                   ^
/usr/include/glm/detail/type_mat4x4.inl:731:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat4x4<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat4x4.hpp:257:0,
                 from /usr/include/glm/mat4x4.hpp:32,
                 from /usr/include/glm/glm.hpp:106,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat4x4.inl:713:35: note: template<class T, glm::precision P> glm::detail::tmat2x4<T, P> glm::detail::operator*(const glm::detail::tmat4x4<T, P>&, const glm::detail::tmat2x4<T, P>&)
  GLM_FUNC_QUALIFIER tmat2x4<T, P> operator*
                                   ^
/usr/include/glm/detail/type_mat4x4.inl:713:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat4x4<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat4x4.hpp:257:0,
                 from /usr/include/glm/mat4x4.hpp:32,
                 from /usr/include/glm/glm.hpp:106,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat4x4.inl:699:54: note: template<class T, glm::precision P> typename glm::detail::tmat4x4<T, P>::row_type glm::detail::operator*(const typename glm::detail::tmat4x4<T, P>::col_type&, const glm::detail::tmat4x4<T, P>&)
  GLM_FUNC_QUALIFIER typename tmat4x4<T, P>::row_type operator*
                                                      ^
/usr/include/glm/detail/type_mat4x4.inl:699:54: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘const ivec4 {aka const glm::detail::tvec4<int, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat4x4<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat4x4.hpp:257:0,
                 from /usr/include/glm/mat4x4.hpp:32,
                 from /usr/include/glm/glm.hpp:106,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat4x4.inl:651:54: note: template<class T, glm::precision P> typename glm::detail::tmat4x4<T, P>::col_type glm::detail::operator*(const glm::detail::tmat4x4<T, P>&, const typename glm::detail::tmat4x4<T, P>::row_type&)
  GLM_FUNC_QUALIFIER typename tmat4x4<T, P>::col_type operator*
                                                      ^
/usr/include/glm/detail/type_mat4x4.inl:651:54: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat4x4<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat4x4.hpp:257:0,
                 from /usr/include/glm/mat4x4.hpp:32,
                 from /usr/include/glm/glm.hpp:106,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat4x4.inl:637:35: note: template<class T, glm::precision P> glm::detail::tmat4x4<T, P> glm::detail::operator*(const T&, const glm::detail::tmat4x4<T, P>&)
  GLM_FUNC_QUALIFIER tmat4x4<T, P> operator*
                                   ^
/usr/include/glm/detail/type_mat4x4.inl:637:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘const ivec4 {aka const glm::detail::tvec4<int, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat4x4<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat4x4.hpp:257:0,
                 from /usr/include/glm/mat4x4.hpp:32,
                 from /usr/include/glm/glm.hpp:106,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat4x4.inl:623:35: note: template<class T, glm::precision P> glm::detail::tmat4x4<T, P> glm::detail::operator*(const glm::detail::tmat4x4<T, P>&, const T&)
  GLM_FUNC_QUALIFIER tmat4x4<T, P> operator*
                                   ^
/usr/include/glm/detail/type_mat4x4.inl:623:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat4x4<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat4x3.hpp:219:0,
                 from /usr/include/glm/mat4x3.hpp:32,
                 from /usr/include/glm/glm.hpp:105,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat4x3.inl:573:35: note: template<class T, glm::precision P> glm::detail::tmat4x3<T, P> glm::detail::operator*(const glm::detail::tmat4x3<T, P>&, const glm::detail::tmat4x4<T, P>&)
  GLM_FUNC_QUALIFIER tmat4x3<T, P> operator*
                                   ^
/usr/include/glm/detail/type_mat4x3.inl:573:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat4x3<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat4x3.hpp:219:0,
                 from /usr/include/glm/mat4x3.hpp:32,
                 from /usr/include/glm/glm.hpp:105,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat4x3.inl:527:35: note: template<class T, glm::precision P> glm::detail::tmat3x3<T, P> glm::detail::operator*(const glm::detail::tmat4x3<T, P>&, const glm::detail::tmat3x4<T, P>&)
  GLM_FUNC_QUALIFIER tmat3x3<T, P> operator*
                                   ^
/usr/include/glm/detail/type_mat4x3.inl:527:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat4x3<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat4x3.hpp:219:0,
                 from /usr/include/glm/mat4x3.hpp:32,
                 from /usr/include/glm/glm.hpp:105,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat4x3.inl:498:54: note: template<class T, glm::precision P> typename glm::detail::tmat4x3<T, P>::row_type glm::detail::operator*(const typename glm::detail::tmat4x3<T, P>::col_type&, const glm::detail::tmat4x3<T, P>&)
  GLM_FUNC_QUALIFIER typename tmat4x3<T, P>::row_type operator*
                                                      ^
/usr/include/glm/detail/type_mat4x3.inl:498:54: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘const ivec4 {aka const glm::detail::tvec4<int, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat4x3<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat4x3.hpp:219:0,
                 from /usr/include/glm/mat4x3.hpp:32,
                 from /usr/include/glm/glm.hpp:105,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat4x3.inl:486:54: note: template<class T, glm::precision P> typename glm::detail::tmat4x3<T, P>::col_type glm::detail::operator*(const glm::detail::tmat4x3<T, P>&, const typename glm::detail::tmat4x3<T, P>::row_type&)
  GLM_FUNC_QUALIFIER typename tmat4x3<T, P>::col_type operator*
                                                      ^
/usr/include/glm/detail/type_mat4x3.inl:486:54: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat4x3<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat4x3.hpp:219:0,
                 from /usr/include/glm/mat4x3.hpp:32,
                 from /usr/include/glm/glm.hpp:105,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat4x3.inl:474:35: note: template<class T, glm::precision P> glm::detail::tmat4x3<T, P> glm::detail::operator*(const T&, const glm::detail::tmat4x3<T, P>&)
  GLM_FUNC_QUALIFIER tmat4x3<T, P> operator* (
                                   ^
/usr/include/glm/detail/type_mat4x3.inl:474:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘const ivec4 {aka const glm::detail::tvec4<int, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat4x3<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat4x3.hpp:219:0,
                 from /usr/include/glm/mat4x3.hpp:32,
                 from /usr/include/glm/glm.hpp:105,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat4x3.inl:462:35: note: template<class T, glm::precision P> glm::detail::tmat4x3<T, P> glm::detail::operator*(const glm::detail::tmat4x3<T, P>&, const T&)
  GLM_FUNC_QUALIFIER tmat4x3<T, P> operator* (
                                   ^
/usr/include/glm/detail/type_mat4x3.inl:462:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat4x3<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat4x2.hpp:219:0,
                 from /usr/include/glm/mat4x2.hpp:32,
                 from /usr/include/glm/glm.hpp:104,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat4x2.inl:541:35: note: template<class T, glm::precision P> glm::detail::tmat2x2<T, P> glm::detail::operator*(const glm::detail::tmat4x2<T, P>&, const glm::detail::tmat2x4<T, P>&)
  GLM_FUNC_QUALIFIER tmat2x2<T, P> operator*
                                   ^
/usr/include/glm/detail/type_mat4x2.inl:541:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat4x2<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat4x3.hpp:219:0,
                 from /usr/include/glm/mat4x3.hpp:32,
                 from /usr/include/glm/glm.hpp:105,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat4x3.inl:511:35: note: template<class T, glm::precision P> glm::detail::tmat2x3<T, P> glm::detail::operator*(const glm::detail::tmat4x3<T, P>&, const glm::detail::tmat2x4<T, P>&)
  GLM_FUNC_QUALIFIER tmat2x3<T, P> operator*
                                   ^
/usr/include/glm/detail/type_mat4x3.inl:511:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat4x3<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat4x2.hpp:219:0,
                 from /usr/include/glm/mat4x2.hpp:32,
                 from /usr/include/glm/glm.hpp:104,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat4x2.inl:590:35: note: template<class T, glm::precision P> glm::detail::tmat4x2<T, P> glm::detail::operator*(const glm::detail::tmat4x2<T, P>&, const glm::detail::tmat4x4<T, P>&)
  GLM_FUNC_QUALIFIER tmat4x2<T, P> operator*
                                   ^
/usr/include/glm/detail/type_mat4x2.inl:590:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat4x2<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat4x2.hpp:219:0,
                 from /usr/include/glm/mat4x2.hpp:32,
                 from /usr/include/glm/glm.hpp:104,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat4x2.inl:574:35: note: template<class T, glm::precision P> glm::detail::tmat3x2<T, P> glm::detail::operator*(const glm::detail::tmat4x2<T, P>&, const glm::detail::tmat3x4<T, P>&)
  GLM_FUNC_QUALIFIER tmat3x2<T, P> operator*
                                   ^
/usr/include/glm/detail/type_mat4x2.inl:574:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat4x2<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat4x2.hpp:219:0,
                 from /usr/include/glm/mat4x2.hpp:32,
                 from /usr/include/glm/glm.hpp:104,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat4x2.inl:527:54: note: template<class T, glm::precision P> typename glm::detail::tmat4x2<T, P>::row_type glm::detail::operator*(const typename glm::detail::tmat4x2<T, P>::col_type&, const glm::detail::tmat4x2<T, P>&)
  GLM_FUNC_QUALIFIER typename tmat4x2<T, P>::row_type operator*
                                                      ^
/usr/include/glm/detail/type_mat4x2.inl:527:54: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘const ivec4 {aka const glm::detail::tvec4<int, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat4x2<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat4x2.hpp:219:0,
                 from /usr/include/glm/mat4x2.hpp:32,
                 from /usr/include/glm/glm.hpp:104,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat4x2.inl:515:54: note: template<class T, glm::precision P> typename glm::detail::tmat4x2<T, P>::col_type glm::detail::operator*(const glm::detail::tmat4x2<T, P>&, const typename glm::detail::tmat4x2<T, P>::row_type&)
  GLM_FUNC_QUALIFIER typename tmat4x2<T, P>::col_type operator*
                                                      ^
/usr/include/glm/detail/type_mat4x2.inl:515:54: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat4x2<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat4x2.hpp:219:0,
                 from /usr/include/glm/mat4x2.hpp:32,
                 from /usr/include/glm/glm.hpp:104,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat4x2.inl:501:35: note: template<class T, glm::precision P> glm::detail::tmat4x2<T, P> glm::detail::operator*(const T&, const glm::detail::tmat4x2<T, P>&)
  GLM_FUNC_QUALIFIER tmat4x2<T, P> operator*
                                   ^
/usr/include/glm/detail/type_mat4x2.inl:501:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘const ivec4 {aka const glm::detail::tvec4<int, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat4x2<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat4x2.hpp:219:0,
                 from /usr/include/glm/mat4x2.hpp:32,
                 from /usr/include/glm/glm.hpp:104,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat4x2.inl:487:35: note: template<class T, glm::precision P> glm::detail::tmat4x2<T, P> glm::detail::operator*(const glm::detail::tmat4x2<T, P>&, const T&)
  GLM_FUNC_QUALIFIER tmat4x2<T, P> operator*
                                   ^
/usr/include/glm/detail/type_mat4x2.inl:487:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat4x2<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat3x4.hpp:213:0,
                 from /usr/include/glm/mat3x4.hpp:32,
                 from /usr/include/glm/glm.hpp:103,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat3x4.inl:570:35: note: template<class T, glm::precision P> glm::detail::tmat3x4<T, P> glm::detail::operator*(const glm::detail::tmat3x4<T, P>&, const glm::detail::tmat3x3<T, P>&)
  GLM_FUNC_QUALIFIER tmat3x4<T, P> operator*
                                   ^
/usr/include/glm/detail/type_mat3x4.inl:570:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat3x4<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat3x4.hpp:213:0,
                 from /usr/include/glm/mat3x4.hpp:32,
                 from /usr/include/glm/glm.hpp:103,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat3x4.inl:552:35: note: template<class T, glm::precision P> glm::detail::tmat2x4<T, P> glm::detail::operator*(const glm::detail::tmat3x4<T, P>&, const glm::detail::tmat2x3<T, P>&)
  GLM_FUNC_QUALIFIER tmat2x4<T, P> operator*
                                   ^
/usr/include/glm/detail/type_mat3x4.inl:552:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat3x4<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat3x4.hpp:213:0,
                 from /usr/include/glm/mat3x4.hpp:32,
                 from /usr/include/glm/glm.hpp:103,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat3x4.inl:499:35: note: template<class T, glm::precision P> glm::detail::tmat4x4<T, P> glm::detail::operator*(const glm::detail::tmat3x4<T, P>&, const glm::detail::tmat4x3<T, P>&)
  GLM_FUNC_QUALIFIER tmat4x4<T, P> operator*
                                   ^
/usr/include/glm/detail/type_mat3x4.inl:499:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat3x4<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat3x4.hpp:213:0,
                 from /usr/include/glm/mat3x4.hpp:32,
                 from /usr/include/glm/glm.hpp:103,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat3x4.inl:486:54: note: template<class T, glm::precision P> typename glm::detail::tmat3x4<T, P>::row_type glm::detail::operator*(const typename glm::detail::tmat3x4<T, P>::col_type&, const glm::detail::tmat3x4<T, P>&)
  GLM_FUNC_QUALIFIER typename tmat3x4<T, P>::row_type operator*
                                                      ^
/usr/include/glm/detail/type_mat3x4.inl:486:54: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘const ivec4 {aka const glm::detail::tvec4<int, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat3x4<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat3x4.hpp:213:0,
                 from /usr/include/glm/mat3x4.hpp:32,
                 from /usr/include/glm/glm.hpp:103,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat3x4.inl:472:54: note: template<class T, glm::precision P> typename glm::detail::tmat3x4<T, P>::col_type glm::detail::operator*(const glm::detail::tmat3x4<T, P>&, const typename glm::detail::tmat3x4<T, P>::row_type&)
  GLM_FUNC_QUALIFIER typename tmat3x4<T, P>::col_type operator*
                                                      ^
/usr/include/glm/detail/type_mat3x4.inl:472:54: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat3x4<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat3x4.hpp:213:0,
                 from /usr/include/glm/mat3x4.hpp:32,
                 from /usr/include/glm/glm.hpp:103,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat3x4.inl:459:35: note: template<class T, glm::precision P> glm::detail::tmat3x4<T, P> glm::detail::operator*(const T&, const glm::detail::tmat3x4<T, P>&)
  GLM_FUNC_QUALIFIER tmat3x4<T, P> operator*
                                   ^
/usr/include/glm/detail/type_mat3x4.inl:459:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘const ivec4 {aka const glm::detail::tvec4<int, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat3x4<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat3x4.hpp:213:0,
                 from /usr/include/glm/mat3x4.hpp:32,
                 from /usr/include/glm/glm.hpp:103,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat3x4.inl:446:35: note: template<class T, glm::precision P> glm::detail::tmat3x4<T, P> glm::detail::operator*(const glm::detail::tmat3x4<T, P>&, const T&)
  GLM_FUNC_QUALIFIER tmat3x4<T, P> operator*
                                   ^
/usr/include/glm/detail/type_mat3x4.inl:446:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat3x4<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat3x3.hpp:250:0,
                 from /usr/include/glm/mat3x3.hpp:32,
                 from /usr/include/glm/glm.hpp:102,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat3x3.inl:669:35: note: template<class T, glm::precision P> glm::detail::tmat4x3<T, P> glm::detail::operator*(const glm::detail::tmat3x3<T, P>&, const glm::detail::tmat4x3<T, P>&)
  GLM_FUNC_QUALIFIER tmat4x3<T, P> operator*
                                   ^
/usr/include/glm/detail/type_mat3x3.inl:669:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat3x3<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat3x3.hpp:250:0,
                 from /usr/include/glm/mat3x3.hpp:32,
                 from /usr/include/glm/glm.hpp:102,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat3x3.inl:653:35: note: template<class T, glm::precision P> glm::detail::tmat2x3<T, P> glm::detail::operator*(const glm::detail::tmat3x3<T, P>&, const glm::detail::tmat2x3<T, P>&)
  GLM_FUNC_QUALIFIER tmat2x3<T, P> operator*
                                   ^
/usr/include/glm/detail/type_mat3x3.inl:653:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat3x3<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat3x3.hpp:250:0,
                 from /usr/include/glm/mat3x3.hpp:32,
                 from /usr/include/glm/glm.hpp:102,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat3x3.inl:613:35: note: template<class T, glm::precision P> glm::detail::tmat3x3<T, P> glm::detail::operator*(const glm::detail::tmat3x3<T, P>&, const glm::detail::tmat3x3<T, P>&)
  GLM_FUNC_QUALIFIER tmat3x3<T, P> operator* 
                                   ^
/usr/include/glm/detail/type_mat3x3.inl:613:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat3x3<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat3x3.hpp:250:0,
                 from /usr/include/glm/mat3x3.hpp:32,
                 from /usr/include/glm/glm.hpp:102,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat3x3.inl:600:54: note: template<class T, glm::precision P> typename glm::detail::tmat3x3<T, P>::row_type glm::detail::operator*(const typename glm::detail::tmat3x3<T, P>::col_type&, const glm::detail::tmat3x3<T, P>&)
  GLM_FUNC_QUALIFIER typename tmat3x3<T, P>::row_type operator* 
                                                      ^
/usr/include/glm/detail/type_mat3x3.inl:600:54: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘const ivec4 {aka const glm::detail::tvec4<int, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat3x3<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat3x3.hpp:250:0,
                 from /usr/include/glm/mat3x3.hpp:32,
                 from /usr/include/glm/glm.hpp:102,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat3x3.inl:587:54: note: template<class T, glm::precision P> typename glm::detail::tmat3x3<T, P>::col_type glm::detail::operator*(const glm::detail::tmat3x3<T, P>&, const typename glm::detail::tmat3x3<T, P>::row_type&)
  GLM_FUNC_QUALIFIER typename tmat3x3<T, P>::col_type operator* 
                                                      ^
/usr/include/glm/detail/type_mat3x3.inl:587:54: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat3x3<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat3x3.hpp:250:0,
                 from /usr/include/glm/mat3x3.hpp:32,
                 from /usr/include/glm/glm.hpp:102,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat3x3.inl:574:35: note: template<class T, glm::precision P> glm::detail::tmat3x3<T, P> glm::detail::operator*(const T&, const glm::detail::tmat3x3<T, P>&)
  GLM_FUNC_QUALIFIER tmat3x3<T, P> operator* 
                                   ^
/usr/include/glm/detail/type_mat3x3.inl:574:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘const ivec4 {aka const glm::detail::tvec4<int, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat3x3<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat3x3.hpp:250:0,
                 from /usr/include/glm/mat3x3.hpp:32,
                 from /usr/include/glm/glm.hpp:102,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat3x3.inl:561:35: note: template<class T, glm::precision P> glm::detail::tmat3x3<T, P> glm::detail::operator*(const glm::detail::tmat3x3<T, P>&, const T&)
  GLM_FUNC_QUALIFIER tmat3x3<T, P> operator* 
                                   ^
/usr/include/glm/detail/type_mat3x3.inl:561:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat3x3<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat3x2.hpp:213:0,
                 from /usr/include/glm/mat3x2.hpp:32,
                 from /usr/include/glm/glm.hpp:101,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat3x2.inl:541:35: note: template<class T, glm::precision P> glm::detail::tmat4x2<T, P> glm::detail::operator*(const glm::detail::tmat3x2<T, P>&, const glm::detail::tmat4x3<T, P>&)
  GLM_FUNC_QUALIFIER tmat4x2<T, P> operator*
                                   ^
/usr/include/glm/detail/type_mat3x2.inl:541:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat3x2<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat3x2.hpp:213:0,
                 from /usr/include/glm/mat3x2.hpp:32,
                 from /usr/include/glm/glm.hpp:101,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat3x2.inl:525:35: note: template<class T, glm::precision P> glm::detail::tmat3x2<T, P> glm::detail::operator*(const glm::detail::tmat3x2<T, P>&, const glm::detail::tmat3x3<T, P>&)
  GLM_FUNC_QUALIFIER tmat3x2<T, P> operator*
                                   ^
/usr/include/glm/detail/type_mat3x2.inl:525:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat3x2<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat3x2.hpp:213:0,
                 from /usr/include/glm/mat3x2.hpp:32,
                 from /usr/include/glm/glm.hpp:101,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat3x2.inl:496:35: note: template<class T, glm::precision P> glm::detail::tmat2x2<T, P> glm::detail::operator*(const glm::detail::tmat3x2<T, P>&, const glm::detail::tmat2x3<T, P>&)
  GLM_FUNC_QUALIFIER tmat2x2<T, P> operator*
                                   ^
/usr/include/glm/detail/type_mat3x2.inl:496:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat3x2<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat3x2.hpp:213:0,
                 from /usr/include/glm/mat3x2.hpp:32,
                 from /usr/include/glm/glm.hpp:101,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat3x2.inl:484:54: note: template<class T, glm::precision P> typename glm::detail::tmat3x2<T, P>::row_type glm::detail::operator*(const typename glm::detail::tmat3x2<T, P>::col_type&, const glm::detail::tmat3x2<T, P>&)
  GLM_FUNC_QUALIFIER typename tmat3x2<T, P>::row_type operator*
                                                      ^
/usr/include/glm/detail/type_mat3x2.inl:484:54: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘const ivec4 {aka const glm::detail::tvec4<int, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat3x2<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat3x2.hpp:213:0,
                 from /usr/include/glm/mat3x2.hpp:32,
                 from /usr/include/glm/glm.hpp:101,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat3x2.inl:473:54: note: template<class T, glm::precision P> typename glm::detail::tmat3x2<T, P>::col_type glm::detail::operator*(const glm::detail::tmat3x2<T, P>&, const typename glm::detail::tmat3x2<T, P>::row_type&)
  GLM_FUNC_QUALIFIER typename tmat3x2<T, P>::col_type operator* 
                                                      ^
/usr/include/glm/detail/type_mat3x2.inl:473:54: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat3x2<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat3x2.hpp:213:0,
                 from /usr/include/glm/mat3x2.hpp:32,
                 from /usr/include/glm/glm.hpp:101,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat3x2.inl:460:35: note: template<class T, glm::precision P> glm::detail::tmat3x2<T, P> glm::detail::operator*(const T&, const glm::detail::tmat3x2<T, P>&)
  GLM_FUNC_QUALIFIER tmat3x2<T, P> operator* 
                                   ^
/usr/include/glm/detail/type_mat3x2.inl:460:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘const ivec4 {aka const glm::detail::tvec4<int, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat3x2<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat3x2.hpp:213:0,
                 from /usr/include/glm/mat3x2.hpp:32,
                 from /usr/include/glm/glm.hpp:101,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat3x2.inl:447:35: note: template<class T, glm::precision P> glm::detail::tmat3x2<T, P> glm::detail::operator*(const glm::detail::tmat3x2<T, P>&, const T&)
  GLM_FUNC_QUALIFIER tmat3x2<T, P> operator* 
                                   ^
/usr/include/glm/detail/type_mat3x2.inl:447:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat3x2<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat2x4.hpp:210:0,
                 from /usr/include/glm/mat2x4.hpp:32,
                 from /usr/include/glm/glm.hpp:100,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat2x4.inl:527:35: note: template<class T, glm::precision P> glm::detail::tmat3x4<T, P> glm::detail::operator*(const glm::detail::tmat2x4<T, P>&, const glm::detail::tmat3x2<T, P>&)
  GLM_FUNC_QUALIFIER tmat3x4<T, P> operator*
                                   ^
/usr/include/glm/detail/type_mat2x4.inl:527:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat2x4<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat2x4.hpp:210:0,
                 from /usr/include/glm/mat2x4.hpp:32,
                 from /usr/include/glm/glm.hpp:100,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat2x4.inl:509:35: note: template<class T, glm::precision P> glm::detail::tmat2x4<T, P> glm::detail::operator*(const glm::detail::tmat2x4<T, P>&, const glm::detail::tmat2x2<T, P>&)
  GLM_FUNC_QUALIFIER tmat2x4<T, P> operator*
                                   ^
/usr/include/glm/detail/type_mat2x4.inl:509:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat2x4<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat2x4.hpp:210:0,
                 from /usr/include/glm/mat2x4.hpp:32,
                 from /usr/include/glm/glm.hpp:100,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat2x4.inl:464:35: note: template<class T, glm::precision P> glm::detail::tmat4x4<T, P> glm::detail::operator*(const glm::detail::tmat2x4<T, P>&, const glm::detail::tmat4x2<T, P>&)
  GLM_FUNC_QUALIFIER tmat4x4<T, P> operator*
                                   ^
/usr/include/glm/detail/type_mat2x4.inl:464:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat2x4<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat2x4.hpp:210:0,
                 from /usr/include/glm/mat2x4.hpp:32,
                 from /usr/include/glm/glm.hpp:100,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat2x4.inl:452:54: note: template<class T, glm::precision P> typename glm::detail::tmat2x4<T, P>::row_type glm::detail::operator*(const typename glm::detail::tmat2x4<T, P>::col_type&, const glm::detail::tmat2x4<T, P>&)
  GLM_FUNC_QUALIFIER typename tmat2x4<T, P>::row_type operator*
                                                      ^
/usr/include/glm/detail/type_mat2x4.inl:452:54: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘const ivec4 {aka const glm::detail::tvec4<int, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat2x4<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat2x4.hpp:210:0,
                 from /usr/include/glm/mat2x4.hpp:32,
                 from /usr/include/glm/glm.hpp:100,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat2x4.inl:438:54: note: template<class T, glm::precision P> typename glm::detail::tmat2x4<T, P>::col_type glm::detail::operator*(const glm::detail::tmat2x4<T, P>&, const typename glm::detail::tmat2x4<T, P>::row_type&)
  GLM_FUNC_QUALIFIER typename tmat2x4<T, P>::col_type operator* 
                                                      ^
/usr/include/glm/detail/type_mat2x4.inl:438:54: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat2x4<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat2x4.hpp:210:0,
                 from /usr/include/glm/mat2x4.hpp:32,
                 from /usr/include/glm/glm.hpp:100,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat2x4.inl:426:35: note: template<class T, glm::precision P> glm::detail::tmat2x4<T, P> glm::detail::operator*(const T&, const glm::detail::tmat2x4<T, P>&)
  GLM_FUNC_QUALIFIER tmat2x4<T, P> operator*
                                   ^
/usr/include/glm/detail/type_mat2x4.inl:426:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘const ivec4 {aka const glm::detail::tvec4<int, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat2x4<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat2x4.hpp:210:0,
                 from /usr/include/glm/mat2x4.hpp:32,
                 from /usr/include/glm/glm.hpp:100,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat2x4.inl:414:35: note: template<class T, glm::precision P> glm::detail::tmat2x4<T, P> glm::detail::operator*(const glm::detail::tmat2x4<T, P>&, const T&)
  GLM_FUNC_QUALIFIER tmat2x4<T, P> operator*
                                   ^
/usr/include/glm/detail/type_mat2x4.inl:414:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat2x4<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat2x3.hpp:208:0,
                 from /usr/include/glm/mat2x3.hpp:32,
                 from /usr/include/glm/glm.hpp:99,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat2x3.inl:508:35: note: template<class T, glm::precision P> glm::detail::tmat4x3<T, P> glm::detail::operator*(const glm::detail::tmat2x3<T, P>&, const glm::detail::tmat4x2<T, P>&)
  GLM_FUNC_QUALIFIER tmat4x3<T, P> operator*
                                   ^
/usr/include/glm/detail/type_mat2x3.inl:508:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat2x3<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat2x3.hpp:208:0,
                 from /usr/include/glm/mat2x3.hpp:32,
                 from /usr/include/glm/glm.hpp:99,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat2x3.inl:474:35: note: template<class T, glm::precision P> glm::detail::tmat3x3<T, P> glm::detail::operator*(const glm::detail::tmat2x3<T, P>&, const glm::detail::tmat3x2<T, P>&)
  GLM_FUNC_QUALIFIER tmat3x3<T, P> operator*
                                   ^
/usr/include/glm/detail/type_mat2x3.inl:474:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat2x3<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat2x3.hpp:208:0,
                 from /usr/include/glm/mat2x3.hpp:32,
                 from /usr/include/glm/glm.hpp:99,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat2x3.inl:458:35: note: template<class T, glm::precision P> glm::detail::tmat2x3<T, P> glm::detail::operator*(const glm::detail::tmat2x3<T, P>&, const glm::detail::tmat2x2<T, P>&)
  GLM_FUNC_QUALIFIER tmat2x3<T, P> operator*
                                   ^
/usr/include/glm/detail/type_mat2x3.inl:458:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat2x3<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat2x3.hpp:208:0,
                 from /usr/include/glm/mat2x3.hpp:32,
                 from /usr/include/glm/glm.hpp:99,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat2x3.inl:447:54: note: template<class T, glm::precision P> typename glm::detail::tmat2x3<T, P>::row_type glm::detail::operator*(const typename glm::detail::tmat2x3<T, P>::col_type&, const glm::detail::tmat2x3<T, P>&)
  GLM_FUNC_QUALIFIER typename tmat2x3<T, P>::row_type operator*
                                                      ^
/usr/include/glm/detail/type_mat2x3.inl:447:54: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘const ivec4 {aka const glm::detail::tvec4<int, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat2x3<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat2x3.hpp:208:0,
                 from /usr/include/glm/mat2x3.hpp:32,
                 from /usr/include/glm/glm.hpp:99,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat2x3.inl:435:54: note: template<class T, glm::precision P> typename glm::detail::tmat2x3<T, P>::col_type glm::detail::operator*(const glm::detail::tmat2x3<T, P>&, const typename glm::detail::tmat2x3<T, P>::row_type&)
  GLM_FUNC_QUALIFIER typename tmat2x3<T, P>::col_type operator*
                                                      ^
/usr/include/glm/detail/type_mat2x3.inl:435:54: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat2x3<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat2x3.hpp:208:0,
                 from /usr/include/glm/mat2x3.hpp:32,
                 from /usr/include/glm/glm.hpp:99,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat2x3.inl:423:35: note: template<class T, glm::precision P> glm::detail::tmat2x3<T, P> glm::detail::operator*(const T&, const glm::detail::tmat2x3<T, P>&)
  GLM_FUNC_QUALIFIER tmat2x3<T, P> operator*
                                   ^
/usr/include/glm/detail/type_mat2x3.inl:423:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘const ivec4 {aka const glm::detail::tvec4<int, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat2x3<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat2x3.hpp:208:0,
                 from /usr/include/glm/mat2x3.hpp:32,
                 from /usr/include/glm/glm.hpp:99,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat2x3.inl:411:35: note: template<class T, glm::precision P> glm::detail::tmat2x3<T, P> glm::detail::operator*(const glm::detail::tmat2x3<T, P>&, const T&)
  GLM_FUNC_QUALIFIER tmat2x3<T, P> operator* 
                                   ^
/usr/include/glm/detail/type_mat2x3.inl:411:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat2x3<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat2x2.hpp:246:0,
                 from /usr/include/glm/mat2x2.hpp:32,
                 from /usr/include/glm/glm.hpp:98,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat2x2.inl:546:35: note: template<class T, glm::precision P> glm::detail::tmat4x2<T, P> glm::detail::operator*(const glm::detail::tmat2x2<T, P>&, const glm::detail::tmat4x2<T, P>&)
  GLM_FUNC_QUALIFIER tmat4x2<T, P> operator*
                                   ^
/usr/include/glm/detail/type_mat2x2.inl:546:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat2x2<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat2x2.hpp:246:0,
                 from /usr/include/glm/mat2x2.hpp:32,
                 from /usr/include/glm/glm.hpp:98,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat2x2.inl:530:35: note: template<class T, glm::precision P> glm::detail::tmat3x2<T, P> glm::detail::operator*(const glm::detail::tmat2x2<T, P>&, const glm::detail::tmat3x2<T, P>&)
  GLM_FUNC_QUALIFIER tmat3x2<T, P> operator*
                                   ^
/usr/include/glm/detail/type_mat2x2.inl:530:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat2x2<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat2x2.hpp:246:0,
                 from /usr/include/glm/mat2x2.hpp:32,
                 from /usr/include/glm/glm.hpp:98,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat2x2.inl:516:35: note: template<class T, glm::precision P> glm::detail::tmat2x2<T, P> glm::detail::operator*(const glm::detail::tmat2x2<T, P>&, const glm::detail::tmat2x2<T, P>&)
  GLM_FUNC_QUALIFIER tmat2x2<T, P> operator*
                                   ^
/usr/include/glm/detail/type_mat2x2.inl:516:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat2x2<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat2x2.hpp:246:0,
                 from /usr/include/glm/mat2x2.hpp:32,
                 from /usr/include/glm/glm.hpp:98,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat2x2.inl:504:54: note: template<class T, glm::precision P> typename glm::detail::tmat2x2<T, P>::row_type glm::detail::operator*(const typename glm::detail::tmat2x2<T, P>::col_type&, const glm::detail::tmat2x2<T, P>&)
  GLM_FUNC_QUALIFIER typename tmat2x2<T, P>::row_type operator*
                                                      ^
/usr/include/glm/detail/type_mat2x2.inl:504:54: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘const ivec4 {aka const glm::detail::tvec4<int, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat2x2<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat2x2.hpp:246:0,
                 from /usr/include/glm/mat2x2.hpp:32,
                 from /usr/include/glm/glm.hpp:98,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat2x2.inl:492:54: note: template<class T, glm::precision P> typename glm::detail::tmat2x2<T, P>::col_type glm::detail::operator*(const glm::detail::tmat2x2<T, P>&, const typename glm::detail::tmat2x2<T, P>::row_type&)
  GLM_FUNC_QUALIFIER typename tmat2x2<T, P>::col_type operator*
                                                      ^
/usr/include/glm/detail/type_mat2x2.inl:492:54: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat2x2<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat2x2.hpp:246:0,
                 from /usr/include/glm/mat2x2.hpp:32,
                 from /usr/include/glm/glm.hpp:98,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat2x2.inl:480:35: note: template<class T, glm::precision P> glm::detail::tmat2x2<T, P> glm::detail::operator*(const T&, const glm::detail::tmat2x2<T, P>&)
  GLM_FUNC_QUALIFIER tmat2x2<T, P> operator*
                                   ^
/usr/include/glm/detail/type_mat2x2.inl:480:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘const ivec4 {aka const glm::detail::tvec4<int, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat2x2<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_mat2x2.hpp:246:0,
                 from /usr/include/glm/mat2x2.hpp:32,
                 from /usr/include/glm/glm.hpp:98,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_mat2x2.inl:468:35: note: template<class T, glm::precision P> glm::detail::tmat2x2<T, P> glm::detail::operator*(const glm::detail::tmat2x2<T, P>&, const T&)
  GLM_FUNC_QUALIFIER tmat2x2<T, P> operator*
                                   ^
/usr/include/glm/detail/type_mat2x2.inl:468:35: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tmat2x2<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_vec4.hpp:373:0,
                 from /usr/include/glm/vec4.hpp:32,
                 from /usr/include/glm/glm.hpp:97,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_vec4.inl:630:33: note: template<class T, glm::precision P> glm::detail::tvec4<T, P> glm::detail::operator*(const glm::detail::tvec4<T, P>&, const glm::detail::tvec4<T, P>&)
  GLM_FUNC_QUALIFIER tvec4<T, P> operator*
                                 ^
/usr/include/glm/detail/type_vec4.inl:630:33: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   deduced conflicting types for parameter ‘T’ (‘float’ and ‘int’)
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_vec4.hpp:373:0,
                 from /usr/include/glm/vec4.hpp:32,
                 from /usr/include/glm/glm.hpp:97,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_vec4.inl:616:33: note: template<class T, glm::precision P> glm::detail::tvec4<T, P> glm::detail::operator*(const T&, const glm::detail::tvec4<T, P>&)
  GLM_FUNC_QUALIFIER tvec4<T, P> operator* 
                                 ^
/usr/include/glm/detail/type_vec4.inl:616:33: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   deduced conflicting types for parameter ‘T’ (‘glm::detail::tvec4<float, (glm::precision)0u>’ and ‘int’)
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_vec4.hpp:373:0,
                 from /usr/include/glm/vec4.hpp:32,
                 from /usr/include/glm/glm.hpp:97,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_vec4.inl:602:33: note: template<class T, glm::precision P> glm::detail::tvec4<T, P> glm::detail::operator*(const glm::detail::tvec4<T, P>&, const T&)
  GLM_FUNC_QUALIFIER tvec4<T, P> operator* 
                                 ^
/usr/include/glm/detail/type_vec4.inl:602:33: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   deduced conflicting types for parameter ‘const T’ (‘float’ and ‘glm::ivec4 {aka glm::detail::tvec4<int, (glm::precision)0u>}’)
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_vec3.hpp:330:0,
                 from /usr/include/glm/vec3.hpp:32,
                 from /usr/include/glm/glm.hpp:96,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_vec3.inl:566:33: note: template<class T, glm::precision P> glm::detail::tvec3<T, P> glm::detail::operator*(const glm::detail::tvec3<T, P>&, const glm::detail::tvec3<T, P>&)
  GLM_FUNC_QUALIFIER tvec3<T, P> operator* 
                                 ^
/usr/include/glm/detail/type_vec3.inl:566:33: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tvec3<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_vec3.hpp:330:0,
                 from /usr/include/glm/vec3.hpp:32,
                 from /usr/include/glm/glm.hpp:96,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_vec3.inl:553:33: note: template<class T, glm::precision P> glm::detail::tvec3<T, P> glm::detail::operator*(const T&, const glm::detail::tvec3<T, P>&)
  GLM_FUNC_QUALIFIER tvec3<T, P> operator* 
                                 ^
/usr/include/glm/detail/type_vec3.inl:553:33: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘const ivec4 {aka const glm::detail::tvec4<int, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tvec3<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_vec3.hpp:330:0,
                 from /usr/include/glm/vec3.hpp:32,
                 from /usr/include/glm/glm.hpp:96,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_vec3.inl:540:33: note: template<class T, glm::precision P> glm::detail::tvec3<T, P> glm::detail::operator*(const glm::detail::tvec3<T, P>&, const T&)
  GLM_FUNC_QUALIFIER tvec3<T, P> operator*
                                 ^
/usr/include/glm/detail/type_vec3.inl:540:33: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tvec3<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_vec2.hpp:312:0,
                 from /usr/include/glm/vec2.hpp:32,
                 from /usr/include/glm/glm.hpp:95,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_vec2.inl:540:33: note: template<class T, glm::precision P> glm::detail::tvec2<T, P> glm::detail::operator*(const glm::detail::tvec2<T, P>&, const glm::detail::tvec2<T, P>&)
  GLM_FUNC_QUALIFIER tvec2<T, P> operator*
                                 ^
/usr/include/glm/detail/type_vec2.inl:540:33: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tvec2<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_vec2.hpp:312:0,
                 from /usr/include/glm/vec2.hpp:32,
                 from /usr/include/glm/glm.hpp:95,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_vec2.inl:528:33: note: template<class T, glm::precision P> glm::detail::tvec2<T, P> glm::detail::operator*(const T&, const glm::detail::tvec2<T, P>&)
  GLM_FUNC_QUALIFIER tvec2<T, P> operator*
                                 ^
/usr/include/glm/detail/type_vec2.inl:528:33: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘const ivec4 {aka const glm::detail::tvec4<int, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tvec2<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /usr/include/glm/detail/type_vec2.hpp:312:0,
                 from /usr/include/glm/vec2.hpp:32,
                 from /usr/include/glm/glm.hpp:95,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/math.h:17,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:31,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/usr/include/glm/detail/type_vec2.inl:516:33: note: template<class T, glm::precision P> glm::detail::tvec2<T, P> glm::detail::operator*(const glm::detail::tvec2<T, P>&, const T&)
  GLM_FUNC_QUALIFIER tvec2<T, P> operator*
                                 ^
/usr/include/glm/detail/type_vec2.inl:516:33: note:   template argument deduction/substitution failed:
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/opengl.h:16:0,
                 from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:47,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/graphic/gui/common.h:282:51: note:   ‘glm::vec4 {aka glm::detail::tvec4<float, (glm::precision)0u>}’ is not derived from ‘const glm::detail::tvec2<T, P>’
                 glm::vec4 lWiseProduct = lColor * mColor;
                                                   ^
In file included from /home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/precompiled.h:60:0,
                 from ../graphic/gui/common.cpp:11:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/utils.h: At global scope:
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/utils.h:28:21: warning: ‘void Framework::Utils::Demangling(const char*, std::string&)’ defined but not used [-Wunused-function]
         static void Demangling(const char* aMangledStr, std::string& aDemangledStr)
                     ^
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/utils.h:44:28: warning: ‘std::string Framework::Utils::GetFileContents(const string&)’ defined but not used [-Wunused-function]
         static std::string GetFileContents(const std::string& aFilePath)
                            ^
/home/mgironesd/workspace/Workspace_Private/interviewtest/src/framework/core/utils.h:66:28: warning: ‘std::string Framework::Utils::GetFileExtension(const string&)’ defined but not used [-Wunused-function]
         static std::string GetFileExtension(const std::string& aFileName)
                            ^
graphic/gui/subdir.mk:45: recipe for target 'graphic/gui/common.o' failed
make: *** [graphic/gui/common.o] Error 1

10:11:17 Build Finished (took 1s.317ms)

