#ifndef ondragc_ASGCTYPEPTRIMPL_H_
#define ondragc_ASGCTYPEPTRIMPL_H_

#include <algorith>
#include <ondragc/asgctypeptrdecl.h>

namespace OndraGC {

/* default trait */
template< bool is_subclass_, typename Object_ >
Object* AsGCTraitDefault< is_subclass_, Object_ >::asGCObject(
    Object_* const object_) {
  return (object_ != 0 ? object_ -> asGCObject() : 0);
}

template< typename Object_ >
Object* AsGCTraitDefault< true, Object_ >::asGCObject(
    Object_* const object_) {
  return object_;
}

/* AsGCTypeRootPtr */
template< typename Object_, typename AsGCTrait >
AsGCTypeRootPtr< Object_, AsGCTrait >::AsGCTypeRootPtr() :
    RootPtr(),
    object(0) {
}

template< typename Object_, typename AsGCTrait >
AsGCTypeRootPtr< Object_, AsGCTrait >::AsGCTypeRootPtr(
    Object_* const object_) :
    RootPtr(AsGCTrait::asGCObject(object_)),
    object(object_) {
}

template< typename Object_, typename AsGCTrait >
AsGCTypeRootPtr< Object_, AsGCTrait >::AsGCTypeRootPtr(
    AsGCTypeRootPtr const & ptr_) :
    RootPtr(ptr_),
    object(ptr_.object) {
}

template< typename Object_, typename AsGCTrait >
void AsGCTypeRootPtr< Object_, AsGCTrait >::swap(
    AsGCTypeRootPtr & ptr_) {
  RootPtr::swap(ptr_);
  std::swap(object, ptr_.object);
}

template< typename Object_, typename AsGCTrait >
AsGCTypeRootPtr< Object_, AsGCTrait >& AsGCTypeRootPtr< Object_, AsGCTrait >::operator =(
    AsGCTypeRootPtr const & ptr_) {
  AsGCTypeRootPtr tmp_(ptr_);
  swap(tmp_);
  return (*this);
}

template< typename Object_, typename AsGCTrait >
AsGCTypeRootPtr< Object_, AsGCTrait >& AsGCTypeRootPtr< Object_, AsGCTrait >::operator =(
    Object_* const object_) {
  AsGCTypeRootPtr tmp_(object_);
  swap(tmp_);
  return *this;
}

template< typename Object_, typename AsGCTrait >
Object_& AsGCTypeRootPtr<Object_, AsGCTrait >::operator *() const {
  return *object;
}

template< typename Object_, typename AsGCTrait >
Object_* AsGCTypeRootPtr<Object_, AsGCTrait >::operator ->() const {
  return object;
}

template< typename Object_, typename AsGCTrait >
Object_* AsGCTypeRootPtr<Object_, AsGCTrait >::objectAddr() const {
  return object;
}

/* AsGCTypeMemberPtr */
template< typename Object_, typename AsGCTrait >
AsGCTypeMemberPtr< Object_, AsGCTrait >::AsGCTypeMemberPtr() :
  MemberPtr(),
  object(0) {

}

template< typename Object_, typename AsGCTrait >
AsGCTypeMemberPtr< Object_, AsGCTrait >::AsGCTypeMemberPtr(
    Object* parent_,
    const MemberPtr::ParentMark & mark_) :
  MemberPtr(parent_, mark_),
  object(0) {

}

template< typename Object_, typename AsGCTrait >
AsGCTypeMemberPtr< Object_, AsGCTrait >::AsGCTypeMemberPtr(
    Object* parent_,
    Object_* const object_) :
  MemberPtr(parent_, AsGCTrait::asGCObject(object_)),
  object(object_) {

}

template< typename Object_, typename AsGCTrait >
AsGCTypeMemberPtr< Object_, AsGCTrait >::AsGCTypeMemberPtr(
    AsGCTypeMemberPtr const & ptr_) :
  MemberPtr(ptr_),
  object(ptr_.object) {

}

template< typename Object_, typename AsGCTrait >
void AsGCTypeMemberPtr< Object_, AsGCTrait >::swap(
    AsGCTypeMemberPtr& ptr_) {
  OndraGCMemberPtr::swap(ptr_);
  std::swap(object, ptr_.object);
}

template< typename Object_, typename AsGCTrait >
AsGCTypeMemberPtr< Object_, AsGCTrait >&
AsGCTypeMemberPtr< Object_, AsGCTrait >::operator =(
    AsGCTypeMemberPtr const & ptr_) {
  AsGCTypeMemberPtr tmp_(ptr_);
  swap(tmp_);
  return *this;
}

template< typename Object_, typename AsGCTrait >
Object_& AsGCTypeMemberPtr< Object_, AsGCTrait >::operator *() const {
  return *object;
}

template< typename Object_, typename AsGCTrait >
Object_* AsGCTypeMemberPtr< Object_, AsGCTrait >::operator ->() const {
  return object;
}

template< typename Object_, typename AsGCTrait >
Object_* AsGCTypeMemberPtr<Object_, AsGCTrait >::objectAddr() const {
  return object;
}

} /* -- namespace OndraGC */

#endif /* ondragc_ASGCTYPEPTRIMPL_H_ */
