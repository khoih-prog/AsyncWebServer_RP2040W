/****************************************************************************************************************************
  StringArray_RP2040W.h

  For RP2040W with CYW43439 WiFi

  AsyncWebServer_RP2040W is a library for the RP2040W with CYW43439 WiFi

  Based on and modified from ESPAsyncWebServer (https://github.com/me-no-dev/ESPAsyncWebServer)
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncWebServer_RP2040W
  Licensed under GPLv3 license

  Version: 1.5.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      13/08/2022 Initial coding for RP2040W with CYW43439 WiFi
  ...
  1.3.0   K Hoang      10/10/2022 Fix crash when using AsyncWebSockets server
  1.3.1   K Hoang      10/10/2022 Improve robustness of AsyncWebSockets server
  1.4.0   K Hoang      20/10/2022 Add LittleFS functions such as AsyncFSWebServer
  1.4.1   K Hoang      10/11/2022 Add examples to demo how to use beginChunkedResponse() to send in chunks
  1.4.2   K Hoang      28/01/2023 Add Async_AdvancedWebServer_SendChunked_MQTT and AsyncWebServer_MQTT_RP2040W examples
  1.5.0   K Hoang      30/01/2023 Fix _catchAllHandler not working bug
 *****************************************************************************************************************************/

#pragma once

#ifndef RP2040W_STRINGARRAY_H_
#define RP2040W_STRINGARRAY_H_

#include "stddef.h"

// deprecated => rp2040/hardware/rp2040/2.4.0/cores/rp2040/api/deprecated/WString.h
//#include "WString.h"

template <typename T>
class LinkedListNode
{
    T _value;

  public:
    LinkedListNode<T>* next;
    LinkedListNode(const T val): _value(val), next(nullptr) {}
    ~LinkedListNode() {}

    /////////////////////////////////////////////////

    inline const T& value() const
    {
      return _value;
    };

    /////////////////////////////////////////////////

    inline T& value()
    {
      return _value;
    }
};

/////////////////////////////////////////////////
/////////////////////////////////////////////////

template <typename T, template<typename> class Item = LinkedListNode>
class LinkedList
{
  public:
    typedef Item<T> ItemType;
    typedef std::function<void(const T&)> OnRemove;
    typedef std::function<bool(const T&)> Predicate;

  private:
    ItemType* _root;
    OnRemove _onRemove;

    class Iterator
    {
        ItemType* _node;

      public:
        Iterator(ItemType* current = nullptr) : _node(current) {}
        Iterator(const Iterator& i) : _node(i._node) {}

        /////////////////////////////////////////////////

        inline Iterator& operator ++()
        {
          _node = _node->next;

          return *this;
        }

        /////////////////////////////////////////////////

        inline bool operator != (const Iterator& i) const
        {
          return _node != i._node;
        }

        /////////////////////////////////////////////////

        inline const T& operator * () const
        {
          return _node->value();
        }

        /////////////////////////////////////////////////

        inline const T* operator -> () const
        {
          return &_node->value();
        }
    };

  public:
    typedef const Iterator ConstIterator;

    /////////////////////////////////////////////////

    inline ConstIterator begin() const
    {
      return ConstIterator(_root);
    }

    /////////////////////////////////////////////////

    inline ConstIterator end() const
    {
      return ConstIterator(nullptr);
    }

    /////////////////////////////////////////////////

    LinkedList(OnRemove onRemove) : _root(nullptr), _onRemove(onRemove) {}

    /////////////////////////////////////////////////

    ~LinkedList() {}

    /////////////////////////////////////////////////

    void add(const T& t)
    {
      auto it = new ItemType(t);

      if (!_root)
      {
        _root = it;
      }
      else
      {
        auto i = _root;

        while (i->next)
          i = i->next;

        i->next = it;
      }
    }

    /////////////////////////////////////////////////

    inline T& front() const
    {
      return _root->value();
    }

    /////////////////////////////////////////////////

    inline bool isEmpty() const
    {
      return _root == nullptr;
    }

    /////////////////////////////////////////////////

    size_t length() const
    {
      size_t i = 0;
      auto it = _root;

      while (it)
      {
        i++;
        it = it->next;
      }

      return i;
    }

    /////////////////////////////////////////////////

    size_t count_if(Predicate predicate) const
    {
      size_t i = 0;
      auto it = _root;

      while (it)
      {
        if (!predicate)
        {
          i++;
        }
        else if (predicate(it->value()))
        {
          i++;
        }

        it = it->next;
      }

      return i;
    }

    /////////////////////////////////////////////////

    const T* nth(size_t N) const
    {
      size_t i = 0;
      auto it = _root;

      while (it)
      {
        if (i++ == N)
          return &(it->value());

        it = it->next;
      }

      return nullptr;
    }

    /////////////////////////////////////////////////

    bool remove(const T& t)
    {
      auto it = _root;
      auto pit = _root;

      while (it)
      {
        if (it->value() == t)
        {
          if (it == _root)
          {
            _root = _root->next;
          }
          else
          {
            pit->next = it->next;
          }

          if (_onRemove)
          {
            _onRemove(it->value());
          }

          delete it;

          return true;
        }

        pit = it;
        it = it->next;
      }

      return false;
    }

    /////////////////////////////////////////////////

    bool remove_first(Predicate predicate)
    {
      auto it = _root;
      auto pit = _root;

      while (it)
      {
        if (predicate(it->value()))
        {
          if (it == _root)
          {
            _root = _root->next;
          }
          else
          {
            pit->next = it->next;
          }

          if (_onRemove)
          {
            _onRemove(it->value());
          }

          delete it;

          return true;
        }

        pit = it;
        it = it->next;
      }

      return false;
    }

    /////////////////////////////////////////////////

    void free()
    {
      while (_root != nullptr)
      {
        auto it = _root;
        _root = _root->next;

        if (_onRemove)
        {
          _onRemove(it->value());
        }

        delete it;
      }

      _root = nullptr;
    }
};

/////////////////////////////////////////////////
/////////////////////////////////////////////////

class StringArray : public LinkedList<String>
{
  public:

    StringArray() : LinkedList(nullptr) {}

    /////////////////////////////////////////////////

    bool containsIgnoreCase(const String& str)
    {
      for (const auto& s : *this)
      {
        if (str.equalsIgnoreCase(s))
        {
          return true;
        }
      }

      return false;
    }
};

#endif /* RP2040W_STRINGARRAY_H_ */
