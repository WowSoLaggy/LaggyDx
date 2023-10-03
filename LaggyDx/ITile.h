#pragma once


namespace Dx
{
  namespace thd
  {
    class ITile
    {
    public:
      virtual ~ITile() = default;

      virtual double getT() const = 0;
      virtual void setT(double i_t) = 0;
    };

  } // ns thd
} // ns Dx
