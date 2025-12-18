#pragma once

#include <utility>

namespace caffeine {

    // is_base_of_v but does not force T to be complete
    /*
    template <typename T>
    concept IsCocosObject = requires(T* obj) {
      { obj->retain() };
      { obj->release() }; 
      { obj->autorelease() } -> std::convertible_to<cocos2d::CCObject*>;
    };
*/
    /**
     * A reference counting utility for CCObjects.
     * Similar to geode::Ref but uses a concept instead of is_base_of_v check.
     *
     * RefNauseam, get it?
     * 
     * @tparam T The type to manage 
     */
    template <class /*IsCocosObject*/ T>
    class RefNauseam final {
        T* m_obj = nullptr;

    public:
        /**
         * Construct a RefNauseam of an object. The object will be retained and
         * managed until RefNauseam goes out of scope
         * @param obj Object to construct the RefNauseam from
         */
        RefNauseam(T* obj) : m_obj(obj) {
            if (m_obj) {
                m_obj->retain();
            }
        }

        RefNauseam(RefNauseam<T> const& other) : RefNauseam(other.data()) {}

        RefNauseam(RefNauseam<T>&& other) noexcept : m_obj(other.m_obj) {
            other.m_obj = nullptr;
        }

        /**
         * Construct an empty RefNauseam (the managed object will be null)
         */
        RefNauseam() = default;

        ~RefNauseam() {
            if (m_obj) {
                m_obj->release();
            }
        }

        /**
         * Swap the managed object with another object. The managed object
         * will be released, and the new object retained
         * @param other The new object to swap to
         */
        void swap(T* other) {
            if (m_obj) {
                m_obj->release();
            }
            m_obj = other;
            if (m_obj) {
                m_obj->retain();
            }
        }

        /**
         * Return the managed object
         * @returns The managed object
         */
        T* data() const {
            return m_obj;
        }

        T* get() const {
            return m_obj;
        }

        operator T*() const {
            return m_obj;
        }

        T* operator*() const {
            return m_obj;
        }

        T* operator->() const {
            return m_obj;
        }

        T* operator=(T* obj) {
            this->swap(obj);
            return obj;
        }

        RefNauseam<T>& operator=(RefNauseam<T> const& other) {
            this->swap(other.data());
            return *this;
        }

        RefNauseam<T>& operator=(RefNauseam<T>&& other) {
            m_obj = other.data();
            other.m_obj = nullptr;
            return *this;
        }

        bool operator==(T* other) const {
            return m_obj == other;
        }

        bool operator==(RefNauseam<T> const& other) const {
            return m_obj == other.m_obj;
        }

        bool operator!=(T* other) const {
            return m_obj != other;
        }

        bool operator!=(RefNauseam<T> const& other) const {
            return m_obj != other.m_obj;
        }

        // for containers
        bool operator<(RefNauseam<T> const& other) const {
            return m_obj < other.m_obj;
        }
    };
}
