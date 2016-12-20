#pragma once

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
	inline Region<K> &operator==(const Region<K> &rhs);

	//quadrants follow top to bottom left to right
	enum Quadrants
	{
		QT_0 = 0, QT_1, QT_2, QT_3, QT_MULT, QT_OUT
	};

	bool within(Region<K> &region);
	Quadrants withinQuadrant(Region<K> &region);
	bool intersect(Region<K> &region);
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
inline Region<K>& Region<K>::operator==(const Region<K>& rhs)
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
	//check bounds
	if (lowRow < region.lowRow || highRow > region.highRow
		|| lowCol < region.lowCol || highCol > region.highCol) {
		//out of bounds
		return Quadrants::QT_OUT;
	}

	K midRow = region.lowRow_ + ((region.highRow_ - Region.lowRow_) / 2);
	K midCol = region.lowCol_ + ((region.highCol_ - Region.lowCol_) / 2);

	if (highRow < midRow) {
		//top half
		if (highCol < midCol) {
			//top left quadrant (0,0)
			return Quadrants::QT_0;
		}
		else {
			//top right quadrant (0,1)
			return Quadrants::QT_1;
		}
	}
	else {
		//bottom half
		if (highCol < midCol) {
			//bottom left quadrant (1,0)
			return Quadrants::QT_2;
		}
		else {
			//bottom right quadrant (1,1)
			Quadrants::QT_3
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



template<typename K, typename T>
class Quadtree
{
public:
	Quadtree(Region<K> &bounds, T &boundsData);
	~Quadtree() = default;
	void insert(Region<K> region);

private:
	struct Node
	{
		Region<K> key;
		T data;
		Node *subtree[4] = { nullptr };
	} root_;
	size_t size_;
};

template<typename K, typename T>
Quadtree<K, T>::Quadtree(Region<K> &bounds, T &boundsData)
{
	root_.key = bounds;
	root_.data = boundsData;
	size_ = 1;
}

template<typename K, typename T>
void Quadtree<K, T>::insert(Region<K> region)
{
	//TODO: implement
}
