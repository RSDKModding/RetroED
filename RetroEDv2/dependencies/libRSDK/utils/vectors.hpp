#ifndef RSDK_VECTORS_H
#define RSDK_VECTORS_H

template <class T> struct Vector2 {
    Vector2() = default;
    Vector2(Reader &reader)
    {
        x = reader.read<T>();
        y = reader.read<T>();
    }
    inline void write(Writer &writer)
    {
        writer.write(x);
        writer.write(y);
    }
    inline Vector2(T X, T Y) : x(X), y(Y) {}
    inline Vector2(T val) : x(val), y(val) {}
    inline Vector2(QVector2D val) : x(val.x()), y(val.y()) {}

    inline QVector2D getVec() { return QVector2D(x, y); }
    inline void fromVec(QVector2D vec)
    {
        x = vec.x();
        y = vec.y();
    }

    void normalize()
    {
        float len = sqrt(x * x + y * y);
        x         = (x / len);
        y         = (y / len);
    }

    void normalize(float mag)
    {
        x /= mag;
        y /= mag;
    }

    void normalize(Vector2<T> vec)
    {
        float len = sqrt(vec.x * vec.x + vec.y * vec.y);
        x         = (vec.x / len);
        y         = (vec.y / len);
    }

    inline float distance() { return sqrt(x * x + y * y); }

    friend inline bool operator==(const Vector2<T> &a, const Vector2<T> &b)
    {
        return a.x == b.x && a.y == b.y;
    }
    friend inline Vector2<T> operator+(Vector2<T> a, const Vector2<T> &b)
    {
        a.x += b.x;
        a.y += b.y;
        return a;
    }

    friend inline Vector2<T> operator+(Vector2<T> a, const T &b)
    {
        a.x += b;
        a.y += b;
        return a;
    }

    friend inline Vector2<T> operator-(Vector2<T> a) { return Vector2<T>(-a.x, -a.y); }

    friend inline Vector2<T> operator-(Vector2<T> a, const Vector2<T> &b)
    {
        a.x -= b.x;
        a.y -= b.y;
        return a;
    }

    friend inline Vector2<T> operator-(Vector2<T> a, const T &b)
    {
        a.x -= b;
        a.y -= b;
        return a;
    }

    friend inline Vector2<T> operator*(Vector2<T> a, const Vector2<T> &b)
    {
        a.x *= b.x;
        a.y *= b.y;
        return a;
    }

    friend inline Vector2<T> operator*(Vector2<T> a, const T &b)
    {
        a.x *= b;
        a.y *= b;
        return a;
    }

    friend inline Vector2<T> operator/(Vector2<T> a, const Vector2<T> &b)
    {
        a.x /= b.x;
        a.y /= b.y;
        return a;
    }

    friend inline Vector2<T> operator/(Vector2<T> a, const T &b)
    {
        a.x /= b;
        a.y /= b;
        return a;
    }

    friend inline Vector2<T> operator+=(Vector2<T> a, const Vector2<T> &b)
    {
        a.x += b.x;
        a.y += b.y;
        return a;
    }

    friend inline Vector2<T> operator+=(Vector2<T> a, const T &b)
    {
        a.x += b;
        a.y += b;
        return a;
    }

    friend inline Vector2<T> operator-=(Vector2<T> a, const Vector2<T> &b)
    {
        a.x -= b.x;
        a.y -= b.y;
        return a;
    }

    friend inline Vector2<T> operator-=(Vector2<T> a, const T &b)
    {
        a.x -= b;
        a.y -= b;
        return a;
    }

    friend inline Vector2<T> operator*=(Vector2<T> a, const Vector2<T> &b)
    {
        a.x *= b.x;
        a.y *= b.y;
        return a;
    }

    friend inline Vector2<T> operator*=(Vector2<T> a, const T &b)
    {
        a.x *= b;
        a.y *= b;
        return a;
    }

    friend inline Vector2<T> operator/=(Vector2<T> a, const Vector2<T> &b)
    {
        a.x /= b.x;
        a.y /= b.y;
        return a;
    }

    friend inline Vector2<T> operator/=(Vector2<T> a, const T &b)
    {
        a.x /= b;
        a.y /= b;
        return a;
    }

    // Multiply a vector by values
    inline void multiply(float X, float Y)
    {
        x *= X;
        y *= Y;
    }
    // Get vector dot product using float
    inline float dotProduct(float X, float Y) const { return x * X + y * Y; }
    // Get vector dot product using vertex
    inline float dotProduct(Vector2<T> v) const { return x * v.x + y * v.y; }

    // Get vector dot product using float
    static inline float dot(float x1, float y1, float x2, float y2) { return x1 * x2 + y1 * y2; }
    // Get vector dot product using vertex
    static inline float dot(Vector2<T> v2, Vector2<T> v3) { return v2.x * v3.x + v2.y * v3.y; }

    T x = 0, y = 0;
};

template <class T> struct Vector3 {
    Vector3() = default;
    Vector3(Reader &reader)
    {
        x = reader.read<T>();
        y = reader.read<T>();
        z = reader.read<T>();
    }
    inline void write(Writer &writer)
    {
        writer.write(x);
        writer.write(y);
        writer.write(z);
    }
    inline Vector3(T X, T Y, T Z) : x(X), y(Y), z(Z) {}
    inline Vector3(T val) : x(val), y(val), z(val) {}
    inline Vector3(QVector3D val) : x(val.x()), y(val.y()), z(val.z()) {}

    inline QVector3D getVec() { return QVector3D(x, y, z); }
    inline void fromVec(QVector3D vec)
    {
        x = vec.x();
        y = vec.y();
        z = vec.z();
    }

    static inline Vector3<T> normalizeVec(Vector3<T> vec)
    {
        float len = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
        return Vector3<T>(vec.x / len, vec.y / len, vec.z / len);
    }

    static inline Vector3<T> crossVec(Vector3<T> vec1, Vector3<T> vec2)
    {
        return Vector3<T>(Vector2<T>::dot(Vector2<T>(vec1.y, -vec2.y), Vector2<T>(vec2.z, vec1.z)),
                          Vector2<T>::dot(Vector2<T>(vec1.z, -vec2.z), Vector2<T>(vec2.x, vec1.x)),
                          Vector2<T>::dot(Vector2<T>(vec1.x, -vec2.x), Vector2<T>(vec2.y, vec1.y)));
    }

    inline void normalize()
    {
        float len = sqrt(x * x + y * y + z * z);
        x         = (x / len);
        y         = (y / len);
        z         = (z / len);
    }

    inline float distance() { return sqrt(x * x + y * y + z * z); }

    friend inline bool operator==(const Vector3<T> &a, const Vector3<T> &b)
    {
        return a.x == b.x && a.y == b.y && a.z == b.z;
    }
    friend inline Vector3<T> operator+(Vector3<T> a, const Vector3<T> &b)
    {
        a.x += b.x;
        a.y += b.y;
        a.z += b.z;
        return a;
    }

    friend inline Vector3<T> operator-(Vector3<T> a) { return Vector3<T>(-a.x, -a.y, -a.z); }

    friend inline Vector3<T> operator-(Vector3<T> a, const Vector3<T> &b)
    {
        a.x -= b.x;
        a.y -= b.y;
        a.z -= b.z;
        return a;
    }

    friend inline Vector3<T> operator*(Vector3<T> a, const Vector3<T> &b)
    {
        a.x *= b.x;
        a.y *= b.y;
        a.z *= b.z;
        return a;
    }

    friend inline Vector3<T> operator/(Vector3<T> a, const Vector3<T> &b)
    {
        a.x /= b.x;
        a.y /= b.y;
        a.z /= b.z;
        return a;
    }

    friend inline Vector3<T> operator+=(Vector3<T> a, const Vector3<T> &b)
    {
        a.x += b.x;
        a.y += b.y;
        a.z += b.z;
        return a;
    }

    friend inline Vector3<T> operator-=(Vector3<T> a, const Vector3<T> &b)
    {
        a.x -= b.x;
        a.y -= b.y;
        a.z -= b.z;
        return a;
    }

    friend inline Vector3<T> operator*=(Vector3<T> a, const Vector3<T> &b)
    {
        a.x *= b.x;
        a.y *= b.y;
        a.z *= b.z;
        return a;
    }

    friend inline Vector3<T> operator/=(Vector3<T> a, const Vector3<T> &b)
    {
        a.x /= b.x;
        a.y /= b.y;
        a.z /= b.z;
        return a;
    }

    // Multiply a vector by values
    inline void multiply(float X, float Y, float Z)
    {
        x *= X;
        y *= Y;
        z *= Z;
    }
    // Get vector dot product using float
    inline float dotProduct(float X, float Y, float Z) const { return x * X + y * Y + z * Z; }
    // Get vector dot product using vertex
    inline float dotProduct(Vector3<T> v) const { return x * v.x + y * v.y + z * v.z; }

    T x = 0, y = 0, z = 0;
};

template <class T> struct Vector4 {
    Vector4() = default;
    Vector4(Reader &reader)
    {
        x = reader.read<T>();
        y = reader.read<T>();
        z = reader.read<T>();
        w = reader.read<T>();
    }
    inline void write(Writer &writer)
    {
        writer.write(x);
        writer.write(y);
        writer.write(z);
        writer.write(w);
    }
    inline Vector4(T X, T Y, T Z, T W) : x(X), y(Y), z(Z), w(W) {}
    inline Vector4(T val) : x(val), y(val), z(val), w(val) {}
    inline Vector4(QVector4D val) : x(val.x()), y(val.y()), z(val.z()), w(val.w()) {}

    inline QVector4D getVec() { return QVector4D(x, y, z, w); }
    inline void fromVec(QVector4D vec)
    {
        x = vec.x();
        y = vec.y();
        z = vec.z();
        w = vec.w();
    }

    void normalize()
    {
        // float len = sqrt(x * x + y * y + z * z + w * w);
        // x         = (normal.x / len);
        // y         = (normal.y / len);
        // z         = (normal.z / len);
        // w         = (normal.w / len);
    }

    inline float distance() { return sqrt(x * x + y * y + z * z + w * w); }

    friend inline bool operator==(const Vector4<T> &a, const Vector4<T> &b)
    {
        return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
    }
    friend inline Vector4<T> operator+(Vector4<T> a, const Vector4<T> &b)
    {
        a.x += b.x;
        a.y += b.y;
        a.z += b.z;
        a.w += b.w;
        return a;
    }

    friend inline Vector4<T> operator-(Vector4<T> a) { return Vector4<T>(-a.x, -a.y, -a.z, -a.w); }

    friend inline Vector4<T> operator-(Vector4<T> a, const Vector4<T> &b)
    {
        a.x -= b.x;
        a.y -= b.y;
        a.z -= b.z;
        a.w -= b.w;
        return a;
    }

    friend inline Vector4<T> operator*(Vector4<T> a, const Vector4<T> &b)
    {
        a.x *= b.x;
        a.y *= b.y;
        a.z *= b.z;
        a.w *= b.w;
        return a;
    }

    friend inline Vector4<T> operator/(Vector4<T> a, const Vector4<T> &b)
    {
        a.x /= b.x;
        a.y /= b.y;
        a.z /= b.z;
        a.w /= b.w;
        return a;
    }

    friend inline Vector4<T> operator+=(Vector4<T> a, const Vector4<T> &b)
    {
        a.x += b.x;
        a.y += b.y;
        a.z += b.z;
        a.w += b.w;
        return a;
    }

    friend inline Vector4<T> operator-=(Vector4<T> a, const Vector4<T> &b)
    {
        a.x -= b.x;
        a.y -= b.y;
        a.z -= b.z;
        a.w -= b.w;
        return a;
    }

    friend inline Vector4<T> operator*=(Vector4<T> a, const Vector4<T> &b)
    {
        a.x *= b.x;
        a.y *= b.y;
        a.z *= b.z;
        a.w *= b.w;
        return a;
    }

    friend inline Vector4<T> operator/=(Vector4<T> a, const Vector4<T> &b)
    {
        a.x /= b.x;
        a.y /= b.y;
        a.z /= b.z;
        a.w /= b.w;
        return a;
    }

    // Multiply a vector by values
    inline void multiply(float X, float Y, float Z, float W)
    {
        x *= X;
        y *= Y;
        z *= Z;
        w *= W;
    }
    // Get vector dot product using float
    inline float dotProduct(float X, float Y, float Z, float W) const
    {
        return x * X + y * Y + z * Z + w * W;
    }
    // Get vector dot product using vertex
    inline float dotProduct(Vector4<T> v) const { return x * v.x + y * v.y + z * v.z + w * v.w; }

    T x = 0, y = 0, z = 0, w = 0;
};

// Not a vector but shut up
template <class T> struct Rect {
    Rect() = default;
    Rect(Reader &reader)
    {
        x = reader.read<T>();
        y = reader.read<T>();
        w = reader.read<T>();
        h = reader.read<T>();
    }

    inline void write(Writer &writer)
    {
        writer.write<T>(x);
        writer.write<T>(y);
        writer.write<T>(w);
        writer.write<T>(h);
    }

    inline Rect(T X, T Y, T W, T H) : x(X), y(Y), w(W), h(H) {}
    template <class U> inline bool contains(Vector2<U> vector)
    {
        // return (vector.x > x && vector.y > y && vector.x < (x + w) && vector.y < (y + h));
        return (vector.x > x && vector.y > y && vector.x <= (x + w) && vector.y <= (y + h));
    }
    inline QRect toQRect()
    {
        QRect r;
        r.setX(x);
        r.setY(y);
        r.setWidth(w);
        r.setHeight(h);
        return r;
    }
    inline QRectF toQRectF()
    {
        QRectF r;
        r.setX(x);
        r.setY(y);
        r.setWidth(w);
        r.setHeight(h);
        return r;
    }

    friend inline bool operator==(const Rect<T> &a, const Rect<T> &b)
    {
        return a.x == b.x && a.y == b.y && a.w == b.w && a.h == b.h;
    }

    T x = 0, y = 0, w = 0, h = 0;
};

#endif //! RSDK_
