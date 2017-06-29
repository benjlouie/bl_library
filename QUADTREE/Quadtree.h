#pragma once

#include <unordered_map>
#include <array>

template<typename K>
struct Region
{
	K yLow_;
	K yHigh_;
	K xLow_;
	K xHigh_;

	Region() = default;
	~Region() = default;
	Region(K xLow, K xHigh, K yLow, K yHigh);
	inline Region<K> &operator= (const Region<K> &rhs);
	inline bool operator==(const Region<K> &rhs) const;

	//quadrants follow left to right, top to bottom
	//QT_0 | QT_1
	//-----|-----
	//QT_2 | QT_3
	enum Quadrants : unsigned
	{
		QT_0 = 0, QT_1, QT_2, QT_3, QT_MULT, QT_OUT
	};

	bool within(const Region<K> &region);
	Quadrants withinQuadrant(Region<K> &region);
	bool intersect(Region<K> &region);
	Region<K> getQuadrant(Quadrants quadrant);
};

template<typename K>
Region<K>::Region(K xLow, K xHigh, K yLow, K yHigh)
{
	if (xLow < xHigh) {
		xLow_ = xLow;
		xHigh_ = xHigh;
	}
	else {
		xLow_ = xHigh;
		xHigh_ = xLow;
	}
	if (yLow < yHigh) {
		yLow_ = yLow;
		yHigh_ = yHigh;
	}
	else {
		yLow_ = yHigh;
		yHigh_ = yLow;
	}
}

template<typename K>
inline Region<K>& Region<K>::operator=(const Region<K>& rhs)
{
	yLow_ = rhs.yLow_;
	yHigh_ = rhs.yHigh_;
	xLow_ = rhs.xLow_;
	xHigh_ = rhs.xHigh_;
	return *this;
}

template<typename K>
inline bool Region<K>::operator==(const Region<K> &rhs) const
{
	return yLow_ == rhs.yLow_
		&& yHigh_ == rhs.yHigh_
		&& xLow_ == rhs.xLow_
		&& xHigh_ == rhs.xHigh_;
}

// returns true if THIS is within region
template<typename K>
inline bool Region<K>::within(const Region<K>& region)
{
	return yLow_ >= region.yLow_
		&& yHigh_ <= region.yHigh_
		&& xLow_ >= region.xLow_
		&& xHigh_ <= region.xHigh_;
}

// returns which quadrant of region THIS is within
// assumes (0,0) is top left, meaning QT_0 = (0,0), QT_1 = (0,1), QT_2 = (1, 0), QT_3 = (1, 1)
template<typename K>
typename Region<K>::Quadrants
Region<K>::withinQuadrant(Region<K>& region)
{
	//check bounds, overlapping the edge counts as out (consistent with overlap of yMid/xMid)
	if (yLow_ <= region.yLow_ || yHigh_ >= region.yHigh_
		|| xLow_ <= region.xLow_ || xHigh_ >= region.xHigh_) {
		//out of bounds
		return Quadrants::QT_OUT;
	}

	K yMid = region.yLow_ + ((region.yHigh_ - region.yLow_) / 2);
	K xMid = region.xLow_ + ((region.xHigh_ - region.xLow_) / 2);

	if (yHigh_ < yMid) {
		//top half
		if (xHigh_ < xMid) {
			//top left quadrant (0,0)
			return Quadrants::QT_0;
		}
		else if (xLow_ > xMid) {
			//top right quadrant (0,1)
			return Quadrants::QT_1;
		}
	}
	else if (yLow_ > yMid) {
		//bottom half
		if (xHigh_ < xMid) {
			//bottom left quadrant (1,0)
			return Quadrants::QT_2;
		}
		else if (xLow_ > xMid) {
			//bottom right quadrant (1,1)
			return Quadrants::QT_3;
		}
	}

	//includes regions that overlap the dividing line
	return Quadrants::QT_MULT;
}

// returns true if THIS intersects region
template<typename K>
bool Region<K>::intersect(Region<K>& region)
{
	return (yLow_ <= region.yHigh_ && yHigh_ >= region.yLow_) 
		&& (xLow_ <= region.xHigh_ && xHigh_ >= region.xLow_);
}

// returns region describing the designated subquadrant of THIS
template<typename K>
Region<K> Region<K>::getQuadrant(Quadrants quadrant)
{
	Region<K> subRegion = *this;
	K yMid = subRegion.yLow_ + (subRegion.yHigh_ - subRegion.yLow_) / 2;
	K xMid = subRegion.xLow_ + (subRegion.xHigh_ - subRegion.xLow_) / 2;
	switch (quadrant)
	{
	case Region<K>::Quadrants::QT_0:
		subRegion.yHigh_ = yMid;
		subRegion.xHigh_ = xMid;
		break;
	case Region<K>::Quadrants::QT_1:
		subRegion.yHigh_ = yMid;
		subRegion.xLow_ = xMid;
		break;
	case Region<K>::Quadrants::QT_2:
		subRegion.yLow_ = yMid;
		subRegion.xHigh_ = xMid;
		break;
	case Region<K>::Quadrants::QT_3:
		subRegion.yLow_ = yMid;
		subRegion.xLow_ = xMid;
		break;
	default:
		break;
	}
	return subRegion;
}

//TODO: could I just add all the region values together and hash that? could be faster
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
			hash<K> hasher;
			size_t hash1 = hasher(region.yLow_);
			size_t hash2 = hasher(region.yHigh_);
			size_t hash3 = hasher(region.xLow_);
			size_t hash4 = hasher(region.xLow_);

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
template<typename K, typename T>
class Quadtree
{
public:
	Quadtree(Region<K> &bounds);
	~Quadtree() = default;

	void insert(Region<K> &region, T &data);
	size_t remove(Region<K> &region, T &data);
	size_t remove(Region<K> &region);
	Region<K> getBounds(void);

	std::vector<std::pair<Region<K>, T>> getRegionsWithin(Region<K> &region);
	std::vector<std::pair<Region<K>, T>> getRegionsIntersect(Region<K> &region);
	std::vector<std::pair<Region<K>, T>> getRegionsAll(void);

private:
	struct Node
	{
		Region<K> quadrantBounds;
		std::unordered_map<Region<K>, std::vector<T>> regions;
		std::array<Node *, 4> subQuadrants = { nullptr, nullptr, nullptr, nullptr };
	} root_;
	Region<K> bounds_;
	size_t size_;

	size_t getRegionsWithinWalk(typename Quadtree<K, T>::Node* root, Region<K>& region, std::vector<std::pair<Region<K>, T>>& foundRegions);
	size_t getRegionsIntersectWalk(typename Quadtree<K, T>::Node* root, Region<K>& region, std::vector<std::pair<Region<K>, T>>& foundRegions);
	size_t getRegionsAllWalk(typename Quadtree<K, T>::Node *root, std::vector<std::pair<Region<K>, T>>& regions);
	Node *getContainingNode(Region<K> &region, Region<K> *resultRegion = nullptr);
};

// creates a quadtree with the given bounds
// any extra information can be put in boundsData
template<typename K, typename T>
Quadtree<K, T>::Quadtree(Region<K> &bounds)
{
	bounds_ = bounds;
	root_.quadrantBounds = bounds;
	size_ = 0;
}

//inserts the given region into the quadtree
template<typename K, typename T>
void Quadtree<K, T>::insert(Region<K> &region, T &data)
{
	typename Quadtree<K, T>::Node *cur = &root_;
	Region<K> treeRegion = bounds_;

	typename Region<K>::Quadrants destinationQuadrant = region.withinQuadrant(treeRegion);
	while (destinationQuadrant != Region<K>::Quadrants::QT_OUT
		&& destinationQuadrant != Region<K>::Quadrants::QT_MULT) {
		typename Quadtree<K, T>::Node **quadrant = nullptr;

		//must be subquadrant, go there directly
		quadrant = &cur->subQuadrants[destinationQuadrant];

		if (!(*quadrant)) {
			//add node if needed
			*quadrant = new typename Quadtree<K, T>::Node;
			(*quadrant)->quadrantBounds = treeRegion.getQuadrant(destinationQuadrant);
		}
		cur = *quadrant;

		treeRegion = cur->quadrantBounds;
		destinationQuadrant = region.withinQuadrant(treeRegion);
	}

	//at the correct region (won't fit in subregion)
	cur->regions[region].push_back(data);
	size_++;
}

//TODO: have remove compress the tree back up as needed
// removes all equivalent regions with equivalent data from the tree
// returns the number of regions removed
template<typename K, typename T>
size_t Quadtree<K, T>::remove(Region<K>& region, T & data)
{
	Node *container = getContainingNode(region);

	std::vector<T>& equalRegions = container->regions[region];
	size_t countRemoved = 0;
	for (auto it = equalRegions.begin(); it != equalRegions.end(); ) {
		if (*it == data) {
			it = equalRegions.erase(it);
			countRemoved++;
		}
		else {
			it++;
		}
	}

	size_ -= countRemoved;
	return countRemoved;
}

// removes all equivalent regions from the tree
// returns the number of regions removed
template<typename K, typename T>
size_t Quadtree<K, T>::remove(Region<K>& region)
{
	Node *container = getContainingNode(region);
	size_t countRemoved = container->regions.erase(region);
	size_ -= countRemoved;
	return countRemoved;
}

// returns a region depicting the outlining bounds of the quadtree
template<typename K, typename T>
Region<K> Quadtree<K, T>::getBounds(void)
{
	return bounds_;
}

template<typename K, typename T>
std::vector<std::pair<Region<K>, T>> Quadtree<K, T>::getRegionsWithin(Region<K>& region)
{
	typename Quadtree<K, T>::Node *topNode = this->getContainingNode(region);
	std::vector<std::pair<Region<K>, T>> foundRegions;

	getRegionsWithinWalk(topNode, region, foundRegions);

	return foundRegions;
}

template<typename K, typename T>
std::vector<std::pair<Region<K>, T>> Quadtree<K, T>::getRegionsIntersect(Region<K>& region)
{
	std::vector<std::pair<Region<K>, T>> foundRegions;

	getRegionsIntersectWalk(&root_, region, foundRegions);

	return foundRegions;
}

template<typename K, typename T>
std::vector<std::pair<Region<K>, T>> Quadtree<K, T>::getRegionsAll(void)
{
	std::vector<std::pair<Region<K>, T>> regions;
	getRegionsAllWalk(&root_, regions);

	return regions;
}

template<typename K, typename T>
size_t Quadtree<K, T>::getRegionsWithinWalk(typename Quadtree<K, T>::Node* root, Region<K>& region, std::vector<std::pair<Region<K>, T>>& foundRegions)
{
	size_t count = 0;

	if (root->quadrantBounds.intersect(region)) {
		//intersects node, check regions in this node and below
		for (pair<Region<K>, std::vector<T>> nodeRegion : root->regions) {
			if (nodeRegion.first.within(region)) {
				for (T data : nodeRegion.second) {
					//append all data
					foundRegions.push_back(std::make_pair(nodeRegion.first, data));
					count++;
				}
			}
		}
		
		for (Node *subQuad : root->subQuadrants) {
			if (subQuad != nullptr) {
				count += getRegionsWithinWalk(subQuad, region, foundRegions);
			}
		}
	}

	return count;
}

template<typename K, typename T>
size_t Quadtree<K, T>::getRegionsIntersectWalk(typename Quadtree<K, T>::Node * root, Region<K>& region, std::vector<std::pair<Region<K>, T>>& foundRegions)
{
	size_t count = 0;

	if (root->quadrantBounds.intersect(region)) {
		//intersects node, check regions in this node and below
		for (pair<Region<K>, std::vector<T>> nodeRegion : root->regions) {
			if (nodeRegion.first.intersect(region)) {
				for (T data : nodeRegion.second) {
					//append all data
					foundRegions.push_back(std::make_pair(nodeRegion.first, data));
					count++;
				}
			}
		}

		for (Node *subQuad : root->subQuadrants) {
			if (subQuad != nullptr) {
				count += getRegionsIntersectWalk(subQuad, region, foundRegions);
			}
		}
	}

	return count;
}

template<typename K, typename T>
size_t Quadtree<K, T>::getRegionsAllWalk(typename Quadtree<K, T>::Node *root, std::vector<std::pair<Region<K>, T>>& regions)
{
	size_t count = 0;
	for (auto nodeRegion : root->regions) {
		for (T data : nodeRegion.second) {
			regions.push_back(std::make_pair(nodeRegion.first, data));
			count++;
		}
	}
	for (Node *subQuad : root->subQuadrants) {
		if (subQuad != nullptr) {
			count += getRegionsAllWalk(subQuad, regions);
		}
	}

	return count;
}

// finds the nearest quadtree node containing the given region
// if the exact node doesn't exists, finds the closest ancestor
// returns pointer to the found quadtree node
// if resultRegion is not null, sets it to the found node's region
template<typename K, typename T>
typename Quadtree<K, T>::Node * 
Quadtree<K, T>::getContainingNode(Region<K> & region, Region<K> *resultRegion)
{
	Node *cur = &root_;
	typename Region<K>::Quadrants quadrant;
	Region<K> curRegion = bounds_;

	quadrant = region.withinQuadrant(curRegion);
	while (quadrant != Region<K>::Quadrants::QT_MULT
		&& quadrant != Region<K>::Quadrants::QT_OUT) {
		// must be one of the quadrants, go there directly
		if (cur->subQuadrants[quadrant] == nullptr) {
			//no subquadrant exists, found closest ancestor node
			break;
		}
		cur = cur->subQuadrants[quadrant];
		curRegion = curRegion.getQuadrant(quadrant);

		quadrant = region.withinQuadrant(curRegion);
	}

	if (resultRegion) {
		*resultRegion = curRegion;
	}

	return cur;
}
