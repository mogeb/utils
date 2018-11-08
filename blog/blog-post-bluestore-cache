The OSDs cache data at many layers. BlueStore is mainly responsible of caching
buffer data and onodes. BlueStore divides the entire cache pool into shards,
for better partitioning and less contention. See `vector<Cache*> cache_shards`. It also has
a list of collections (aka PGs) its managing (ie PGs managed by that OSD). These
are stored in `coll_map`. When the OSD boots, it first creates
`num_op_shards` caching shards. It then reads the RocksDB
database to reconstruct its state (ie its collections). This happens in
_open_collections(). In that function, collections are reconstructed and assigned
a shard of cache:
```
CollectionRef c(
	new Collection(
	  this,
	  cache_shards[cid.hash_to_shard(cache_shards.size())],
	  cid));
```
The second argument required by the constructor is a pointer to the shard of cache
that collection should use. What is a Cache object? Depending on whether the OSD
is configured to use an LRU cache or a 2Q cache, the behavior changes, but the idea
is mainly the same: a Cache stores in its internal datastructures both onodes
and buffers (actual data). Both of these containers are initially empty.
// redundant
// A single instance of the Cache class is used for caching onodes and buffers.

- When data is read by the objectstore, in BlueStore::_do_read(), it is cached (assuming
buestore_default_buffered_read):

```
if (buffered) {
	bptr->shared_blob->bc.did_read(bptr->shared_blob->get_cache(), 0,
				       raw_bl);
}
```
`did_read()` will in turn create a Buffer, copy the read data to that newly created
buffer, and finally add it to the buffer datastructure held in the cache (bptr->shared_blob->get_cache()):
```
void did_read(Cache* cache, uint32_t offset, bufferlist& bl) {
	[...]
    Buffer *b = new Buffer(this, Buffer::STATE_CLEAN, 0, offset, bl);
    b->cache_private = _discard(cache, offset, bl.length());
    _add_buffer(cache, b, 1, nullptr);
}
```
_add_buffer() will add the Buffer `b` to the buffer container of the Cache.

- When data is written by the objectstore, in BlueSotre::_do_write_small()
or ::_do_write_big(), coming from
BlueStore::_write() =>
	BlueStore::_do_write() =>
		[BlueStore::_do_write_data(), BlueStore::_do_alloc_write()]
_do_alloc_write() ends up being called whether its a small or big write, which
copies the bufferlist to the Cache via the BufferSpace::write() function:
```
void _buffer_cache_write(...) {
    b->shared_blob->bc.write(b->shared_blob->get_cache(), txc->seq, offset, bl,
			     flags);
    txc->shared_blobs_written.insert(b->shared_blob);
  }
```
And bc.write() (AKA BufferSpace::write()):
```
void write(Cache* cache, uint64_t seq, uint32_t offset, bufferlist& bl,
		unsigned flags) {
	[...]
	Buffer *b = new Buffer(this, Buffer::STATE_WRITING, seq, offset, bl,
				flags);
	b->cache_private = _discard(cache, offset, bl.length());
	_add_buffer(cache, b, (flags & Buffer::FLAG_NOCACHE) ? 0 : 1, nullptr);
}
```
`write()` and `did_read()` are quite similar.


## Who owns the Buffer objects?
In BufferSpace::write(), a Buffer is created with the contents of the bufferlist
that is being written. Then in BufferSpace::write() => cache->_add_buffer(), the
Buffer is added to the internal strucutre *by copy*.


## How can the Cache be accessed around BlueStore?
Any of the Cache shards can be accessed from various places around BlueStore. Each
Cache shard is referenced at multiple locations, and hence easily accessible. Let's
look at the diagram below that shows some important classes of BlueStore as well
as the relations between them.
![alt text](https://github.com/mogeb/utils/blob/master/blog/bluestore_caches.jpg)
As mentioned previously, each Collection is associated with a Cache (see the
Cache *cache attribute) that is assigned at the construction of
a Cache object. BlueStore itself has a container of collections (coll_map), so by
iterating over coll_map, we can access each Collection's Cache. We can also
access it directly via the cache_shards container, which is more straightforward.
Moreover, each Collection has a set of SharedBlob that it manages. Each SharedBlob
also keeps a reference to the Collection it is a part of, which makes it easy to
access the Cache. This is what happens when you call SharedBlob::get_cache() method.
Note that all Blob is a SharedBlob. Each collection also has a set of all onodes
its managing. That set is onode_map. Each onode in that hashmap has a set of the Extents
that belong to it. Each Extent has in turn a reference to a Blob.

An interesting function to look at is BlueStore::Collection::split_cache(), which
is the bulk of the BlueStore::_split_collection().

The function basically goes over all onodes of a certain Collection via onode_map.
For each onode, it removes it from its internal datastructure of onodes.
```
auto p = onode_map.onode_map.begin();
while (p != onode_map.onode_map.end()) {
    OnodeRef o = p->second;
	[...]
	cache->_rm_onode(p->second);
	p = onode_map.onode_map.erase(p);
```
It then sets that onode's Collection reference to the destination Collection,
and adds it the internal datastructure of onodes of the destination Collection's cache,
and finally adds it to the destination Collection's onode_map, making it officially
managed by the destination Collection:
```
      o->c = dest;
      dest->cache->_add_onode(o, 1);
      dest->onode_map.onode_map[o->oid] = o;
      dest->onode_map.cache = dest->cache;
```
We also said that each Collection has a set of Blob it manages, stored via the
shared_blob_set container. We also have to update that datastructure, removing
Blobs from it (1) and adding them to the destionation Collection's shared_blob_set (2).
And all SharedBlobs also have a reference to the collection they belong
to, that reference should be updated too (3):
```
[...]
for (auto sb : sbvec) {
	[...]
	shared_blob_set.remove(sb); (1)
	dest->shared_blob_set.add(dest, sb); (2)
	[...]
	sb->coll = dest; (3)
}
```
There is some other magic happening in that function, but this is the bulk of it.
There is a catch though, because Buffers go in the cache as they''re being written,
it is possible that the write operation isn't completed yet for a particular blob.
Hence, we should make sure that a Blob isn't in the process of being written
as we move it from one Collection to the other. We can use the `is_writing()`
function for that check:
```
for (auto& i : sb->bc.buffer_map) {
	if (!i.second->is_writing()) {
		dest->cache->_move_buffer(cache, i.second.get());
	}
}
```
