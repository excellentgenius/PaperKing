#pragma once

namespace ci_ext
{

//#define PI 3.1415926535897323

  //VisualStudo2012�ȑO�ɂ� nullptr �������̂Œ�`
#if (_MSC_VER<1700)
  const class {
  public:
      template<class T>
      operator T*() const { return 0; }
      // ������^�̃����o�|�C���^�ւ̕ϊ�
      template<class C, class T>
      operator T C::*() const  { return 0; }
  private:
      void operator&() const;
  } nullptr;
#endif

}