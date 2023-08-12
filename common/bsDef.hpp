#include <stdint.h>
#include <vector>
template<class TData>
class Bit {
public:
    Bit() {
        m_bitPos = 0;
        m_bitLen = 0;
    }
    Bit(const TData& data) {
        m_data = data;
    }
    Bit(const Bit& b) {
        this->m_data = b.m_data;
        this->m_bitPos = b.m_bitPos;
        this->m_bitData = b.m_bitData;
        this->m_bitLen = b.m_bitLen;
    }
    TData& operator = (const Bit<TData>& b) {
        return this->m_data;
    }
    TData operator = (Bit<TData>& b) {
        return this->m_data;
    }
    Bit<TData> & operator = (const TData& b) {
        this->m_data = b;
        return *this;
    }
    const bool operator==(const TData& other)const{
        return this->m_data == other;
    }
    const bool operator!=(const TData& other) const{
        return this->m_data != other;
    }
    const TData operator|=(const TData& other) {
        return this->m_data |= other;
    }
    const TData operator-=(const TData& other) {
        return this->m_data -= other;
    }
    const TData operator+=(const TData& other) {
        return this->m_data += other;
    }
    const TData operator<<(const TData& other) const {
        return this->m_data << other;
    }
    const TData operator-(const TData& other) const {
        return this->m_data - other;
    }
    const TData operator*(const TData& other) const {
        return this->m_data * other;
    }
    const TData operator/(const TData& other) const {
        return this->m_data / other;
    }
    const bool operator!() const {
        return !this->m_data;
    }
    
    TData get() const { return m_data; }
    size_t getBitPos() const {return m_bitPos;}
    void setBitPos(size_t pos) {m_bitPos = pos;}
    size_t getBitLen() const {return m_bitLen;}
    void setBitLen(size_t len){ m_bitLen = len;}
    std::vector<uint8_t> getBitData()const {return m_bitData;}
    void setBitData(std::vector<int8_t>& data) {m_bitData = data;}

private:
    TData m_data;
    std::vector<uint8_t> m_bitData;
    int16_t m_bitPos;
    int16_t m_bitLen;

};
#define BIT(type) Bit<##type>
