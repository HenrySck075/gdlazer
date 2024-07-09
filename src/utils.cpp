float randomFloat() {
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

bool randomBool() {
	return randomFloat() >= 0.5;
}