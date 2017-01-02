#pragma once

#include <unordered_map>

template<typename K>
struct Region
{
	K lowRow_;
	K highRow_;
	K lowCol_;
	K highCol_;

	Region(void) = default;
	~Region(void) = default;
	Region(K lowRow, K highRow, K lowCol, K highCol);
	inline Region<K> &operator= (const Region<K> &rhs);
	inline bool operator==(const Region<K> &rhs) const;

	//quadrants follow top to bottom left to right
	enum Quadrants : unsigned
	{
		QT_0 = 0, QT_1, QT_2, QT_3, QT_MULT, QT_OUT
	};

	bool within(Region<K> &region);
	Quadrants withinQuadrant(Region<K> &region);
	bool intersect(Region<K> &region);
	Region<K> getQuadrant(Quadrants quadrant);
};

template<typename K>
Region<K>::Region(K lowRow, K highRow, K lowCol, K highCol)
{
	if (lowRow < highRow) {
		lowRow_ = lowRow;
		highRow_ = highRow;
	}
	else {
		lowRow_ = highRow;
		highRow_ = lowRow;
	}

	if (lowCol < highCol) {
		lowCol_ = lowCol;
		highCol_ = highCol;
	}
	else {
		lowCol_ = highCol;
		highCol_ = lowCol;
	}
}

template<typename K>
inline Region<K>& Region<K>::operator=(const Region<K>& rhs)
{
	lowRow_ = rhs.lowRow_;
	highRow_ = rhs.highRow_;
	lowCol_ = rhs.lowCol_;
	highCol_ = rhs.highCol_;
	return *this;
}

template<typename K>
inline bool Region<K>::operator==(const Region<K> &rhs) const
{
	return lowRow_ == rhs.lowRow_
		&& highRow_ == rhs.highRow_
		&& lowCol_ == rhs.lowCol_
		&& highCol_ == rhs.highCol_;
}

template<typename K>
bool Region<K>::within(Region<K>& region)
{
	return lowRow_ >= region.lowRow_
		&& highRow_ <= region.highRow_
		&& lowCol >= region.lowCol_
		&& highCol_ <= region.highCol_;
}

//assumes (0,0) is top left, meaning QT_0 = (0,0), QT_1 = (0,1), QT_2 = (1, 0), QT_3 = (1, 1)
template<typename K>
typename Region<K>::Quadrants
Region<K>::withinQuadrant(Region<K>& region)
{
	//check bounds, overlapping the edge counts as out (consistent with overlap of midRow/midCol)
	if (lowRow_ <= region.lowRow_ || highRow_ >= region.highRow_
		|| lowCol_ <= region.lowCol_ || highCol_ >= region.highCol_) {
		//out of bounds
		return Quadrants::QT_OUT;
	}

	K midRow = region.lowRow_ + ((region.highRow_ - region.lowRow_) / 2);
	K midCol = region.lowCol_ + ((region.highCol_ - region.lowCol_) / 2);

	if (highRow_ < midRow) {
		//top half
		if (highCol_ < midCol) {
			//top left quadrant (0,0)
			return Quadrants::QT_0;
		}
		else if (lowCol_ > midCol) {
			//top right quadrant (0,1)
			return Quadrants::QT_1;
		}
	}
	else if (lowRow_ > midRow) {
		//bottom half
		if (highCol_ < midCol) {
			//bottom left quadrant (1,0)
			return Quadrants::QT_2;
		}
		else if (lowCol_ > midCol) {
			//bottom right quadrant (1,1)
			return Quadrants::QT_3;
		}
	}

	//includes regions that overlap the dividing line
	return Quadrants::QT_MULT;
}

template<typename K>
bool Region<K>::intersect(Region<K>& region)
{
	return (lowRow_ <= region.highRow_ && highRow_ >= region.lowRow_) 
		&& (lowCol_ <= region.highCol_ && highCol_ >= region.lowCol_);
}

template<typename K>
Region<K> Region<K>::getQuadrant(Quadrants quadrant)
{
	Region<K> subRegion = *this;
	K midRow = subRegion.lowRow_ + (subRegion.highRow_ - subRegion.lowRow_) / 2;
	K midCol = subRegion.lowCol_ + (subRegion.highCol_ - subRegion.lowCol_) / 2;
	switch (quadrant)
	{
	case Region<K>::Quadrants::QT_0:
		subRegion.highRow_ = midRow;
		subRegion.highCol_ = midCol;
		break;
	case Region<K>::Quadrants::QT_1:
		subRegion.highRow_ = midRow;
		subRegion.lowCol_ = midCol;
		break;
	case Region<K>::Quadrants::QT_2:
		subRegion.lowRow_ = midRow;
		subRegion.highCol_ = midCol;
		break;
	case Region<K>::Quadrants::QT_3:
		subRegion.lowRow_ = midRow;
		subRegion.lowCol_ = midCol;
		break;
	default:
		break;
	}
	return subRegion;
}

//Hash function for Region<K>
namespace std {

	template <typename K>
	struct hash<Region<K>>
	{
		std::size_t operator()(const Region<K> &region) const
		{
			using std::size_t;
			using std::hash;
			using std::string;

			// Compute individual hash values for first
			size_t hash1 = hash<K>()(region.lowRow_);
			size_t hash2 = hash<K>()(region.highRow_);
			size_t hash3 = hash<K>()(region.lowCol_);
			size_t hash4 = hash<K>()(region.lowCol_);

			//combine hash values using the method in boost::hash_combine
			size_t hashFinal = hash1;
			hashFinal ^= hash2 + 0x9e3779b9 + (hashFinal << 6) + (hashFinal >> 2);
			hashFinal ^= hash3 + 0x9e3779b9 + (hashFinal << 6) + (hashFinal >> 2);
			hashFinal ^= hash4 + 0x9e3779b9 + (hashFinal << 6) + (hashFinal >> 2);

			return hashFinal;
		}
	};

}

//////////////////////////////////////////////////
//TODO: have a bucket size so the tree doesn't grow more than it needs to
//// when implemented, should go down the tree regardless if the cur node has a child
//////////////////////////////////////////////////

// K - region type
// T - data type
// BD - type for data about the bounds
template<typename K, typename T, typename BD = T>
class Quadtree
{
public:
	Quadtree(Region<K> &bounds, BD &boundsData);
	~Quadtree() = default;
	void insert(Region<K> &region, T &data);

private:
	struct Node
	{
#ifdef _DEBUG
		Region<K> quadrantBounds;
#endif // DEBUG

		std::unordered_map<Region<K>, T> regions;
		Node *subQuadrants[4] = { nullptr, nullptr, nullptr, nullptr };
	} root_;
	Region<K> bounds_;
	BD boundsData_;
	size_t size_;
};

template<typename K, typename T, typename BD>
Quadtree<K, T, BD>::Quadtree(Region<K> &bounds, BD &boundsData)
{
	bounds_ = bounds;
	boundsData_ = boundsData;
#ifdef _DEBUG
	root_.quadrantBounds = bounds;
#endif // DEBUG

	size_ = 0;
}

template<typename K, typename T, typename BD>
void Quadtree<K, T, BD>::insert(Region<K> &region, T &data)
{
	typename Quadtree<K, T, BD>::Node *cur = &root_;
	Region<K> treeRegion = bounds_;

	while (cur != nullptr) { //TODO: don't need comparison, figure out way to redo loop
		Quadtree<K, T, BD>::Node **quadrant = nullptr;

		typename Region<K>::Quadrants destinationQuadrant = region.withinQuadrant(treeRegion);
		switch (destinationQuadrant)
		{
		case Region<K>::Quadrants::QT_OUT:
		case Region<K>::Quadrants::QT_MULT:
			//won't fit in subquadrant, put in current region
			cur->regions[region] = data;
			size_++;
			return;
			break;
		case Region<K>::Quadrants::QT_0:
		case Region<K>::Quadrants::QT_1:
		case Region<K>::Quadrants::QT_2:
		case Region<K>::Quadrants::QT_3:
			quadrant = &cur->subQuadrants[destinationQuadrant];
			break;
		//TODO: could I combine QT_#s with default? ignore possible error?
		default:
			//TODO: error of some kind, shouldn't be possible
			return;
			break;
		}

		if (*quadrant == nullptr) {
			//add node
			*quadrant = new typename Quadtree<K, T, BD>::Node;
#ifdef _DEBUG
			(*quadrant)->quadrantBounds = treeRegion.getQuadrant(destinationQuadrant);
#endif // DEBUG

			cur = *quadrant;
		}
		else {
			//node already exists
			cur = *quadrant;
		}
		treeRegion = treeRegion.getQuadrant(destinationQuadrant);
	}
}