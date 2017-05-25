#ifndef ondragc_ASGCTYPEPTRDECL_H_
#define ondragc_ASGCTYPEPTRDECL_H_

#include <loki/TypeManip.h>
#include <ondragc/object.h>
#include <ondragc/pointer.h>

namespace OndraGC {

/**
 * @brief Default AsGCTrait
 */
template< bool is_subclass_, typename Object_ > class AsGCTraitDefault {
  public:
    static Object* asGCObject(
        Object_* const object_);
};

/**
 * @brief Default AsGCTrait, specialization for subclasses of OndraGCObject
 */
template< typename Object_ > class AsGCTraitDefault< true, Object_ > {
  public:
    static Object* asGCObject(
        Object_* const object_);
};

/**
 * @brief Template object holder as GC root pointer
 */
template< typename Object_, typename AsGCTrait = AsGCTraitDefault<
    ::Loki::SuperSubclass< Object, Object_ >::value, Object_ > >
class AsGCTypeRootPtr : public RootPtr {
  private:
    Object_ * object; /**< Object pointer */

  public:
    /**
     * @brief Ctor
     */
    AsGCTypeRootPtr();
    /**
     * @brief Ctor
     * @param object_ Object
     */
    AsGCTypeRootPtr(
        Object_* const object_);
    /**
     * @brief Copy ctor
     * @param ptr_ Object
     */
    AsGCTypeRootPtr(
        AsGCTypeRootPtr const & ptr_);

    void swap(
        AsGCTypeRootPtr & ptr_);
    AsGCTypeRootPtr & operator =(
        AsGCTypeRootPtr const & ptr_);
    AsGCTypeRootPtr & operator =(
        Object_* const object_);

    Object_& operator *() const;
    Object_* operator ->() const;
    Object_* objectAddr() const;
};

/**
 * @brief Template object holder as GC member pointer
 */
template< typename Object_, typename AsGCTrait = AsGCTraitDefault<
    ::Loki::SuperSubclass< Object, Object_ >::value, Object_ > >
class AsGCTypeMemberPtr : public MemberPtr {
  private:
    Object_* object; /**< Object pointer */

  public:
    /**
     * @brief Ctor
     */
    AsGCTypeMemberPtr();
    /**
     * Ctor - with parent mark
     * @param parent_ Parent
     */
    AsGCTypeMemberPtr(
        Object* parent_,
        const ParentMark &);
    /**
     * @brief Ctor
     * @param parent_ Parent
     * @param object_ Object
     */
    AsGCTypeMemberPtr(
        Object* parent_,
        Object_* const object_);
    /**
     * @brief Copy ctor
     * @param ptr_ Object
     */
    AsGCTypeMemberPtr(
        AsGCTypeMemberPtr const& ptr_);

    void swap(
        AsGCTypeMemberPtr& ptr_);
    AsGCTypeMemberPtr& operator =(
        AsGCTypeMemberPtr const& ptr_);

    Object_& operator *() const;
    Object_* operator ->() const;
    Object_* objectAddr() const;
};

} /* -- namespace OndraGC */

#endif /* ondragc_ASGCTYPEPTRDECL_H_ */
